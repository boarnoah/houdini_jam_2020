// Fill out your copyright notice in the Description page of Project Settings.


#include "HdnEscapeObjective.h"
#include "HdnCharacter.h"
#include "HdnGameMode.h"
#include "HdnGameState.h"
#include "Components/BoxComponent.h"

class AHdnCharacter;
// Sets default values
AHdnEscapeObjective::AHdnEscapeObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	InteractZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractZone"));
	InteractZone->SetupAttachment(GetRootComponent());

	InteractZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractZone->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AHdnEscapeObjective::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHdnEscapeObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHdnEscapeObjective::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AHdnCharacter* player = Cast<AHdnCharacter>(OtherActor);

	if (player && !ObjectiveActivated)
	{
		const auto GS = GetWorld()->GetAuthGameMode<AHdnGameMode>()->GetGameState<AHdnGameState>();

		if (GS->State == EGameObjectiveState::Escape)
		{
			UE_LOG(LogTemp, Log, TEXT("Player activated escape objective"));
			ObjectiveActivated = true;
			player->ActivatedEscape(this);
		}
	}
}