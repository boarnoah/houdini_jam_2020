// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HdnGameMode.generated.h"

class AHdnObjectiveManager;
UCLASS(minimalapi)
class AHdnGameMode : public AGameModeBase
{
	GENERATED_BODY()
	void SpawnObjectives();
public:
	AHdnGameMode();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	AHdnObjectiveManager* ObjectiveManager;

	void RegisterObjectiveManager(AHdnObjectiveManager* flagManager);
	void ActivateObjective(AHdnFlag* objective);
};



