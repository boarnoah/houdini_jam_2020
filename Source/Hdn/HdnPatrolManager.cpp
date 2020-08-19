// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnPatrolManager.h"
#include "HdnPatrol.h"

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
}

// Called every frame
void AHdnPatrolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}