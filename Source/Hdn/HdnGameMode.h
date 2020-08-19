// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HdnGameMode.generated.h"

class AHdnPatrolManager;
class AHdnCharacter;
class AHdnEscapeObjective;
class AHdnFlag;
class AHdnObjectiveManager;
UCLASS(minimalapi)
class AHdnGameMode : public AGameModeBase
{
	GENERATED_BODY()
	void SpawnObjectives();
	void SpawnEscape();
	void EnableEscape() const;
public:
	AHdnGameMode();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	AHdnObjectiveManager* ObjectiveManager;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	AHdnPatrolManager* PatrolManager;

	void RegisterObjectiveManager(AHdnObjectiveManager* flagManager);
	void RegisterPatrolManager(AHdnPatrolManager* patrolManager);
	void ActivateFlagObjective(AHdnFlag* objective);
	void ActivateEscapeObjective(AHdnEscapeObjective* objective);
	void ActivateFeral(AHdnCharacter* player);
};



