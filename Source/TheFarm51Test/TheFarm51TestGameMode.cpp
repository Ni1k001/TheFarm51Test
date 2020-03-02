// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "TheFarm51TestGameMode.h"
#include "TheFarm51TestHUD.h"
#include "TheFarm51TestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATheFarm51TestGameMode::ATheFarm51TestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATheFarm51TestHUD::StaticClass();
}