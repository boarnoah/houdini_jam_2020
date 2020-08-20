// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnPatrolPoint.h"

#include "Components/BoxComponent.h"

// Sets default values
AHdnPatrolPoint::AHdnPatrolPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PatrolBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PatrolBox"));
	PatrolBox->SetBoxExtent(FVector(200, 200, 100));
	SetRootComponent(PatrolBox);
}

// Called when the game starts or when spawned
void AHdnPatrolPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHdnPatrolPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

