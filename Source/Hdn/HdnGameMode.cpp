// Copyright Epic Games, Inc. All Rights Reserved.

#include "HdnGameMode.h"
#include "HdnCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHdnGameMode::AHdnGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
