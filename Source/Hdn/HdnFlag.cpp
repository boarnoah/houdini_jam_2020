// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnFlag.h"

#include "HdnCharacter.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHdnFlag::AHdnFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	InteractZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractZone"));
	InteractZone->SetupAttachment(GetRootComponent());

	InteractZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void AHdnFlag::SetObjectiveEnabled(bool enabled)
{
	ObjectiveEnabled = enabled;
	Mesh->SetVisibility(enabled);
}

void AHdnFlag::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AHdnCharacter* player = Cast<AHdnCharacter>(OtherActor);

	if (player && !ObjectiveActivated)
	{
		UE_LOG(LogTemp, Log, TEXT("Player activated objective"));
		ObjectiveActivated = true;
		player->ActivatedObjective(this);
	}
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

