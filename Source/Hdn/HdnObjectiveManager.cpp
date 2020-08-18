// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnObjectiveManager.h"

#include "HdnGameMode.h"

// Sets default values
AHdnObjectiveManager::AHdnObjectiveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHdnObjectiveManager::BeginPlay()
{
	Super::BeginPlay();
	auto Gm = Cast<AHdnGameMode>( GetWorld()->GetAuthGameMode());
	Gm->RegisterObjectiveManager(this);
}

// Called every frame
void AHdnObjectiveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

