// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerWeaponState : uint8 {
	NoWeapon UMETA(DisplayName = "No Weapon"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Sniper UMETA(DisplayName = "Sniper")
};

UENUM(BlueprintType)
enum class EPlayerActionState : uint8 {
	Neutral UMETA(DisplayName = "Neutral"),
	Attacking UMETA(DisplayName = "Attacking"),
	Aiming UMETA(DisplayName = "Aiming")
};
