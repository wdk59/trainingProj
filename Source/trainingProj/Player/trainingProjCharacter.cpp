// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/trainingProjCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "trainingProj.h"

AtrainingProjCharacter::AtrainingProjCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.f);
		
	// Don't rotate when the controller rotates.
	// Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	// Make character to Look at the direction of character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	// Initialize characterMovement-related variables
	// These can be tweaked in the Character Blueprint
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->TargetOffset = FVector(0.f, 0.f, 70.f);
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm (CameraBoom is rotating based on the controller)
	FollowCamera->SetAutoActivate(true);

	// Create a first person camera
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(GetMesh(), "head");
	FPSCamera->SetRelativeLocation(FVector(0.f, 20.f, 0.f));
	FPSCamera->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	FPSCamera->bUsePawnControlRotation = true;	// Camera always faces the same direction as the character mesh
	FPSCamera->SetAutoActivate(false);

	// Set Character Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PLAYER_MESH_BASIC(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple"));
	if (PLAYER_MESH_BASIC.Succeeded()) {
		GetMesh()->SetSkeletalMesh(PLAYER_MESH_BASIC.Object);
		GetMesh()->SetWorldLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}

	// Action Mapping - Input Action
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JUMP(TEXT("/Game/MyContents/Player/Input/Actions/IA_Jump.IA_Jump"));
	if (IA_JUMP.Succeeded()) {
		JumpAction = IA_JUMP.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE(TEXT("/Game/MyContents/Player/Input/Actions/IA_Move.IA_Move"));
	if (IA_MOVE.Succeeded()) {
		MoveAction = IA_MOVE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_SWITCHVIEW(TEXT("/Game/MyContents/Player/Input/Actions/IA_SwitchView.IA_SwitchView"));
	if (IA_SWITCHVIEW.Succeeded()) {
		SwitchViewAction = IA_SWITCHVIEW.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LOOK(TEXT("/Game/MyContents/Player/Input/Actions/IA_Look.IA_Look"));
	if (IA_LOOK.Succeeded()) {
		LookAction = IA_LOOK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOUSELOOK(TEXT("/Game/MyContents/Player/Input/Actions/IA_MouseLook.IA_MouseLook"));
	if (IA_MOUSELOOK.Succeeded()) {
		MouseLookAction = IA_MOUSELOOK.Object;
	}
}

void AtrainingProjCharacter::BeginPlay() {
	Super::BeginPlay();

	// Set Perspection to TPS
	if (bIsFPS) {
		bIsFPS = false;
	}
	SwitchView();
}

void AtrainingProjCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AtrainingProjCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AtrainingProjCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AtrainingProjCharacter::Look);

		// Switch View (TPS<->FPS)
		EnhancedInputComponent->BindAction(SwitchViewAction, ETriggerEvent::Started, this, &AtrainingProjCharacter::SwitchView);

		UE_LOG(LogTemp, Warning, TEXT("EnhancedInputComponent bound actions"));
	}
	else
	{
		UE_LOG(LogtrainingProj, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AtrainingProjCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp, Log, TEXT("Possessed by Controller: %s"), *NewController->GetName());
}

void AtrainingProjCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AtrainingProjCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AtrainingProjCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AtrainingProjCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AtrainingProjCharacter::SwitchView() {
	if (bIsFPS) {
		FollowCamera->SetActive(true);
		FPSCamera->SetActive(false);

	}
	else {
		FollowCamera->SetActive(false);
		FPSCamera->SetActive(true);
	}

	bIsFPS = !bIsFPS;
}

void AtrainingProjCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AtrainingProjCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
