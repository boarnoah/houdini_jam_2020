#pragma once
#include "EGameObjectiveState.generated.h"

UENUM(BlueprintType)
enum class EGameObjectiveState: uint8
{
	Collection,
	Escape,
	Win,
	Lose
};
