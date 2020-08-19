// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnPatrol.h"
#include "HdnEScout.h"
#include "HdnEGunship.h"
#include "HdnPatrolManager.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AHdnPatrol::AHdnPatrol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetBoxExtent(FVector(1200, 1200, 200));
	SetRootComponent(SpawnBox);
}

void AHdnPatrol::SetPatrolEnabled(bool status)
{
	Enabled = status;

	if (Enabled && FirstEnable)
	{
		UE_LOG(LogTemp, Log, TEXT("Patrol first time activation, spawning enemies"));
		FirstEnable = false;
		SpawnEnemies();
	}

	for (auto Gunship : Gunships)
	{
		ToggleEnemyActive(Gunship, status);
	}

	for (auto Scout : Scouts)
	{
		ToggleEnemyActive(Scout, status);
	}
}

// Called when the game starts or when spawned
void AHdnPatrol::BeginPlay()
{
	Super::BeginPlay();

	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Patrol has no patrol points"));
	}

	AHdnPatrolManager* Pm = Cast<AHdnPatrolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AHdnPatrolManager::StaticClass()));

	if (Pm)
	{
		Pm->RegisterPatrol(this);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Patrol Manager not found"));
	}
}

void AHdnPatrol::SpawnEnemies()
{
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ActorSpawnParams.Owner = this;

	if (Gunships.Num() < MaxGunships)
	{
		const int numGunshipsToSpawn = MaxGunships - Gunships.Num();

		for(int i = 0; i < numGunshipsToSpawn; i++)
		{
			FVector spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(SpawnBox->GetComponentLocation(), SpawnBox->GetScaledBoxExtent());
			AHdnEGunship* gunship = GetWorld()->SpawnActor<AHdnEGunship>(GunshipClass, spawnPoint, FRotator(0, 0, 0), ActorSpawnParams);
			Gunships.Add(gunship);
		}
	}

	if (Scouts.Num() < MaxScouts)
	{
		const int numScoutsToSpawn = MaxScouts - Scouts.Num();

		for(int i = 0; i < numScoutsToSpawn; i++)
		{
			FVector spawnPoint = UKismetMathLibrary::RandomPointInBoundingBox(SpawnBox->GetComponentLocation(), SpawnBox->GetScaledBoxExtent());
			AHdnEScout* scout = GetWorld()->SpawnActor<AHdnEScout>(ScoutClass, spawnPoint, FRotator(0, 0, 0), ActorSpawnParams);
			Scouts.Add(scout);
		}
	}
}

// https://unrealcpp.com/hide-and-disable-actor/
void AHdnPatrol::ToggleEnemyActive(AActor* enemy, bool newState)
{
	// Hides visible components
	enemy->SetActorHiddenInGame(!newState);

	// Disables collision components
	enemy->SetActorEnableCollision(newState);

	// Stops the Actor from ticking
	enemy->SetActorTickEnabled(newState);
}

// Called every frame
void AHdnPatrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

