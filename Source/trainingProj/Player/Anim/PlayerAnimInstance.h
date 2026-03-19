// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/PlayerEnumGroup.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TRAININGPROJ_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EPlayerWeaponState CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Action")
	EPlayerActionState CurrentAction;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
