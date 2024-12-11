// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangeaGameMode.h"
#include "PangeaPlayerController.h"
#include "PangeaCharacter.h"
#include "UObject/ConstructorHelpers.h"

APangeaGameMode::APangeaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APangeaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerAvatar"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}