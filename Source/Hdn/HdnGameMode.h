// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HdnGameMode.generated.h"

class AHdnFlagManager;
UCLASS(minimalapi)
class AHdnGameMode : public AGameModeBase
{
	GENERATED_BODY()
	void SpawnObjectives();
public:
	AHdnGameMode();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	AHdnFlagManager* FlagManager;

	void RegisterFlagManager(AHdnFlagManager* flagManager);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int NumObjectives = 2;
};



