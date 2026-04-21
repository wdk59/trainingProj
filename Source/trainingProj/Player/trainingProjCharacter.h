// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerEnumGroup.h"
#include "trainingProjCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AtrainingProjCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** First camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FPSCamera;

	/** Weapons */
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* SniperMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* RifleMesh;
	
protected:
	/** Gameplay initialization */
	virtual void BeginPlay() override;

public:

	/** Constructor */
	AtrainingProjCharacter();

	EPlayerWeaponState PastWeapon;
	EPlayerWeaponState CurrentWeapon;
	EPlayerActionState CurrentAction;

protected:
	/** Initialize input action bindings */
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Jump;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Move;

	/** Switch View(Camera) Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_SwitchView;

	/** Toggle(Equip/Unequip) Weapon Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_ToggleWeapon;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Look;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_MouseLook;

	/** Choose Gun */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_ChooseRifle;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_ChooseSniper;

	/** Gun Features */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_GunFire;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_GunZoom;

private:
	/** View Status */
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bIsFPS = false;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for setting view */
	void SetView(bool curView);
	/** Called for switching view */
	void SwitchView();
	/** Called for equipping or unequipping weapon */
	void ToggleWeapon();

	/** IA: Choose Gun */
	void OnChooseRifle(const FInputActionValue& Value);
	void OnChooseSniper(const FInputActionValue& Value);
	/** IA: Gun Features */
	void OnGunFire(const FInputActionValue& Value);
	void OnGunZoomIn(const FInputActionValue& Value);
	void OnGunZoomOut(const FInputActionValue& Value);

	void SetWeaponVisibility(const EPlayerWeaponState weapon);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Initialize Character's setting */
	UFUNCTION(BlueprintCallable, Category = "Init")
	void Initialize();
	UFUNCTION(BlueprintCallable, Category = "Init")
	void Initialize_GunSettings();
};

