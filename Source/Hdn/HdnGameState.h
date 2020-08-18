// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HdnGameState.generated.h"

class AHdnFlag;
/**
 * 
 */
UCLASS()
class HDN_API AHdnGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	TArray<AHdnFlag*> Objectives;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int NumObjectives = 2;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int NumActivatedObjectives = 0;
};
