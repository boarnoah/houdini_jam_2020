#pragma once
#include "EEnemyState.generated.h"

UENUM(BlueprintType)
enum class EEnemyState: uint8
{
	Patrol,
	Alerted,
	Combat
};
