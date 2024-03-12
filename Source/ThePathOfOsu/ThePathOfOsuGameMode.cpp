// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThePathOfOsuGameMode.h"
#include "ThePathOfOsuCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThePathOfOsuGameMode::AThePathOfOsuGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerCharacter/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
