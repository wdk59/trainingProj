// Copyright Epic Games, Inc. All Rights Reserved.

#include "Gameplay/trainingProjGameMode.h"
#include "Player/trainingProjCharacter.h"
#include "Player/trainingProjPlayerController.h"

AtrainingProjGameMode::AtrainingProjGameMode()
{
	// Player Controller
	PlayerControllerClass = AtrainingProjPlayerController::StaticClass();

	// Default Pawn
	DefaultPawnClass = AtrainingProjCharacter::StaticClass();
}
