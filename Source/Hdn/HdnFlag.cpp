// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnFlag.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHdnFlag::AHdnFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}


void AHdnFlag::SetObjectiveEnabled(const bool enabled)
{
	ObjectiveEnabled = enabled;
	Mesh->SetVisibility(enabled);
}

// Called when the game starts or when spawned
void AHdnFlag::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHdnFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

