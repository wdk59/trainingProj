// Copyright Epic Games, Inc. All Rights Reserved.


#include "Player/trainingProjPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "trainingProj.h"
#include "Widgets/Input/SVirtualJoystick.h"

AtrainingProjPlayerController::AtrainingProjPlayerController() {
	// Action Mapping - Input Mapping Context : Default Mode
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DEFAULT_CONTEXT(TEXT("/Game/MyContents/Player/Input/IMC_Default.IMC_Default"));
	if (DEFAULT_CONTEXT.Succeeded()) {
		DefaultMappingContexts.Add(DEFAULT_CONTEXT.Object);
	}
	// Action Mapping - Input Mapping Context: Mouse
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MOUSE_CONTEXT(TEXT("'/Game/MyContents/Player/Input/IMC_MouseLook.IMC_MouseLook'"));
	if (MOUSE_CONTEXT.Succeeded()) {
		DefaultMappingContexts.Add(MOUSE_CONTEXT.Object);
	}
}

void AtrainingProjPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogtrainingProj, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AtrainingProjPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			// Clear exist mapping
			Subsystem->ClearAllMappings();

			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}

void AtrainingProjPlayerController::OnPossess(APawn *InPawn) {
	Super::OnPossess(InPawn);
	
	// 만약 컨트롤하는 폰 바뀌면 입력 매핑도 새로 연결해주기
}
