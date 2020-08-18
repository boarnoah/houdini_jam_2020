// Copyright Epic Games, Inc. All Rights Reserved.

#include "HdnGameMode.h"


#include "HdnFlag.h"
#include "HdnObjectiveManager.h"
#include "HdnGameState.h"
#include "UObject/ConstructorHelpers.h"

AHdnGameMode::AHdnGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	GameStateClass = AHdnGameState::StaticClass();
}

void AHdnGameMode::RegisterObjectiveManager(AHdnObjectiveManager* flagManager)
{
	UE_LOG(LogTemp, Log, TEXT("Registered flag manager"));
	ObjectiveManager = flagManager;

	for (AHdnFlag *Objective : ObjectiveManager->ObjectiveFlags)
	{
		Objective->SetObjectiveEnabled(false);
	}
	
	SpawnObjectives();
}

void AHdnGameMode::SpawnObjectives()
{
	// We don't have enough obj positions to spawn requested number of Objs
	if (ObjectiveManager->ObjectiveFlags.Num() < GS->NumObjectives)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough Objective Flags to spawn Required Num Objectives"));
		return;
	}

	AHdnGameState* GS = GetGameState<AHdnGameState>();
	int NumActivated = 0;

	// Select a random set of flags to use for this session's objectives
	do
	{
		const int RandIndex = FMath::RandRange(0, ObjectiveManager->ObjectiveFlags.Num() - 1);

		if (!ObjectiveManager->ObjectiveFlags[RandIndex]->GetObjectiveEnabled())
		{
			ObjectiveManager->ObjectiveFlags[RandIndex]->SetObjectiveEnabled(true);
			GS->Objectives.Add(ObjectiveManager->ObjectiveFlags[RandIndex]);
			NumEnabled++;
		}
	} while (NumActivated < NumObjectives);

	UE_LOG(LogTemp, Log, TEXT("Selected flags"));
}

