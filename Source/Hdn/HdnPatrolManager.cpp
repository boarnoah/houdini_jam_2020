// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnPatrolManager.h"

#include "HdnGameMode.h"
#include "HdnPatrol.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHdnPatrolManager::AHdnPatrolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AHdnPatrolManager::RegisterPatrol(AHdnPatrol* patrol)
{
	patrol->SetPatrolEnabled(false);
	Patrols.Add(patrol);
}

// Called when the game starts or when spawned
void AHdnPatrolManager::BeginPlay()
{
	Super::BeginPlay();
	auto Gm = Cast<AHdnGameMode>( GetWorld()->GetAuthGameMode());
	Gm->RegisterPatrolManager(this);
	PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	GetWorldTimerManager().SetTimer(TimerTickHandle, this, &AHdnPatrolManager::TimerTick, 1.0f, true, -1);
	UE_LOG(LogTemp, Log, TEXT("Started patrol Manager"));
}

void AHdnPatrolManager::TimerTick()
{
	const FVector playerLocation = PlayerCharacter->GetActorLocation();
	for (AHdnPatrol* patrol : Patrols)
	{
		float distanceToPlayer =  FVector::Dist(playerLocation, patrol->GetActorLocation());

		if (distanceToPlayer <= EnemyPatrolActivateDistance)
		{
			patrol->SetPatrolEnabled(true);
		}
	}
}

// Called every frame
void AHdnPatrolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}