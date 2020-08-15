// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnFlagManager.h"

#include "HdnGameMode.h"

// Sets default values
AHdnFlagManager::AHdnFlagManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHdnFlagManager::BeginPlay()
{
	Super::BeginPlay();
	auto Gm = Cast<AHdnGameMode>( GetWorld()->GetAuthGameMode());
	Gm->RegisterFlagManager(this);
}

// Called every frame
void AHdnFlagManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

