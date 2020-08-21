// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnEGunship.h"


#include "DrawDebugHelpers.h"
#include "HdnCharacter.h"
#include "HdnPatrol.h"
#include "HdnPatrolPoint.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"
// Sets default values
AHdnEGunship::AHdnEGunship()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->OnSeePawn.AddDynamic(this, &AHdnEGunship::OnPlayerSeen);

	// Allow Default AI Controller to posses and operate when spawned at runtime
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AHdnEGunship::BeginPlay()
{
	Super::BeginPlay();
	ResumePatrol();
}


void AHdnEGunship::OnPlayerSeen(APawn* Pawn)
{
	if (Pawn == nullptr)
		return;


	AHdnCharacter* PlayerPawn = Cast<AHdnCharacter>(Pawn);
	if (PlayerPawn)
	{
		LastSeenPosition = PlayerPawn->GetActorLocation();

		if (State == EEnemyState::Patrol)
		{
			DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 15.0f, 12, FColor::Yellow, false, 1.0f);

			State = EEnemyState::Alerted;
			InAlertGracePeriod = true;
			GetWorldTimerManager().SetTimer(AlertHandle, this, &AHdnEGunship::OnAlertExpired, AlertedTime, false, -1);
			GetWorldTimerManager().SetTimer(AlertGraceHandle, this, &AHdnEGunship::OnAlertGracePeriodExpired, AlertGracePeriod, false, -1);
			LookAtAlert();
		}
		

		if (State == EEnemyState::Alerted && !InAlertGracePeriod)
		{
			State = EEnemyState::Combat;
			GetWorldTimerManager().SetTimer(WeaponCooldownHandle, this, &AHdnEGunship::OnWeaponCoolDown, WeaponCooldown, true, -1);
			GetWorldTimerManager().SetTimer(CombatCooldownHandle, this, &AHdnEGunship::OnCombatCooldown, CombatCooldown, false, -1);
		}

		// Saw player, remaining in combat
		if (State == EEnemyState::Combat)
		{
			GetWorldTimerManager().ClearTimer(CombatCooldownHandle);
			GetWorldTimerManager().SetTimer(CombatCooldownHandle, this, &AHdnEGunship::OnCombatCooldown, CombatCooldown, false, -1);
		}
	}
}

void AHdnEGunship::OnAlertGracePeriodExpired()
{
	InAlertGracePeriod = false;
	InvestigateAlert();
}

void AHdnEGunship::OnAlertExpired()
{
	if (State != EEnemyState::Combat )
	{
		UE_LOG(LogTemp, Log, TEXT("Alert over back to patrol"));
		State = EEnemyState::Patrol;
		ResumePatrol();
	}
}

void AHdnEGunship::LookAtAlert()
{
	GetController()->StopMovement();
}

void AHdnEGunship::InvestigateAlert()
{
	const auto playerDirection  = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LastSeenPosition).GetNormalized();
	const FVector distance = LastSeenPosition - GetActorLocation();
	const FVector destination = GetActorLocation() + playerDirection.Vector() * (distance.Size() / 2);
	CurrentDestination = GetPointOnNavMesh(destination);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), CurrentDestination);
	// DrawDebugSphere(GetWorld(), CurrentDestination, 15.0f, 12, FColor::Red, false, 10.0f);
	// UE_LOG(LogTemp, Log, TEXT("Investigating alert"));
}

void AHdnEGunship::OnWeaponCoolDown()
{
	if (State != EEnemyState::Combat)
	{
		return;
	}

	const auto player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	const bool hasLoS = PawnSensing->HasLineOfSightTo(player);

	if (hasLoS)
	{
		TargetPoint = player->GetActorLocation() + player->GetActorForwardVector() * (player->GetVelocity() * WeaponFireTime);
	} else
	{
		TargetPoint = LastSeenPosition;
	}

	const FVector distance = TargetPoint - player->GetActorLocation();

	if (distance.Size() <= WeaponDamageRadius)
	{
		player->TakeDamage(WeaponDamage, FDamageEvent(), GetController(), this);
	}

	// DrawDebugSphere(GetWorld(), TargetPoint, WeaponDamageRadius, 12, FColor::Red, false, 10.0f);
}

void AHdnEGunship::OnCombatCooldown()
{
	State = EEnemyState::Alerted;
	UE_LOG(LogTemp, Log, TEXT("Lost sight of enemy in combat, in high alert"));
	InAlertGracePeriod = false;
	GetWorldTimerManager().SetTimer(AlertHandle, this, &AHdnEGunship::OnAlertExpired, AlertedTime, false, -1);
}

void AHdnEGunship::SetNextWaypoint()
{
	if (CurrentWaypointIndex + 1 >= Patrol->PatrolPoints.Num())
	{
		CurrentWaypointIndex = 0;
	}

	CurrentWaypointIndex++;
}

void AHdnEGunship::ResumePatrol()
{
	UBoxComponent* patrolPoint = Patrol->PatrolPoints[CurrentWaypointIndex]->PatrolBox;
	const FVector patrolDestination = UKismetMathLibrary::RandomPointInBoundingBox(
		patrolPoint->GetComponentLocation(), patrolPoint->GetScaledBoxExtent());

	CurrentDestination = GetPointOnNavMesh(patrolDestination);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), CurrentDestination);
	// DrawDebugSphere(GetWorld(), patrolDestination, 15.0f, 12, FColor::Green, false, 10.0f);
	// DrawDebugSphere(GetWorld(), CurrentDestination, 15.0f, 12, FColor::Blue, false, 10.0f);
}

FVector AHdnEGunship::GetPointOnNavMesh(FVector point) const
{
	FNavLocation location;
	const FNavAgentProperties* navAgentProperties = &GetNavAgentPropertiesRef();
	UNavigationSystemV1* navSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());

	if (navSystem == nullptr)
	{
		return point;
	}

	navSystem->ProjectPointToNavigation(point, location, FVector(50, 50, 1000), navAgentProperties);
	return location.Location;
}


// Called every frame
void AHdnEGunship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const FVector distance = CurrentDestination - GetActorLocation();

	if (distance.Size() <= WaypointReachDistance) {
		if (State == EEnemyState::Patrol)
		{
			SetNextWaypoint();
			ResumePatrol();
		}
	}
}

// Called to bind functionality to input
void AHdnEGunship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

