// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Death_MAZEGameMode.h"
#include "Death_MAZECharacter.h"
#include "UObject/ConstructorHelpers.h"

ADeath_MAZEGameMode::ADeath_MAZEGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_DeathMazeCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
