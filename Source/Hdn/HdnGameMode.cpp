// Copyright Epic Games, Inc. All Rights Reserved.

#include "HdnGameMode.h"


#include "HdnEscapeObjective.h"
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

void AHdnGameMode::ActivateFlagObjective(AHdnFlag* objective)
{
	AHdnGameState* GS = GetGameState<AHdnGameState>();
	GS->NumActivatedObjectives++;

	if (GS->NumActivatedObjectives == GS->NumObjectives)
	{
		UE_LOG(LogTemp, Log, TEXT("All objectives activated, time to leave"));
		GS->State = EGameObjectiveState::Escape;
	}
}

void AHdnGameMode::ActivateEscapeObjective(AHdnEscapeObjective* objective)
{
	AHdnGameState* GS = GetGameState<AHdnGameState>();
	UE_LOG(LogTemp, Log, TEXT("Successfully escaped, game won"));
	GS->State = EGameObjectiveState::Win;
}

void AHdnGameMode::ActivateFeral(AHdnCharacter* player)
{
	AHdnGameState* GS = GetGameState<AHdnGameState>();
	UE_LOG(LogTemp, Log, TEXT("Mouse gone feral, you've lost control, game over"));
	GS->State = EGameObjectiveState::Lose;
}

void AHdnGameMode::SpawnObjectives()
{
	AHdnGameState* GS = GetGameState<AHdnGameState>();
	// We don't have enough obj positions to spawn requested number of Objs
	if (ObjectiveManager->ObjectiveFlags.Num() < GS->NumObjectives)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough Objective Flags to spawn Required Num Objectives"));
		return;
	}

	int NumEnabled = 0;

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
	} while (NumEnabled < GS->NumObjectives);

	UE_LOG(LogTemp, Log, TEXT("Selected flags"));
}

