// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EGameObjectiveState.h"
#include "HdnEscapeObjective.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay")
	AHdnEscapeObjective* EscapeObjective;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int NumObjectives = 2;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int NumActivatedObjectives = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	EGameObjectiveState State = EGameObjectiveState::Collection;
};
