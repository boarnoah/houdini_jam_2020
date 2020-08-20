// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnEGunship.h"


#include "DrawDebugHelpers.h"
#include "HdnCharacter.h"
#include "HdnPatrol.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/TargetPoint.h"
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
			GetController()->StopMovement();
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
	UE_LOG(LogTemp, Log, TEXT("Looking at alert"));
}

void AHdnEGunship::InvestigateAlert()
{
	UE_LOG(LogTemp, Log, TEXT("Investigating alert"));
}

void AHdnEGunship::OnWeaponCoolDown()
{
	if (State == EEnemyState::Combat)
	{
		UE_LOG(LogTemp, Log, TEXT("Cooldown finished, firing"));
	}
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
	CurrentDestination = Patrol->PatrolPoints[CurrentWaypointIndex]->GetActorLocation();
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), CurrentDestination);
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

