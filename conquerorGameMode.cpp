// Copyright Epic Games, Inc. All Rights Reserved.

#include "conquerorGameMode.h"
#include "conquerorCharacter.h"
#include "UObject/ConstructorHelpers.h"

AconquerorGameMode::AconquerorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
