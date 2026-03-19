// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Anim/PlayerAnimInstance.h"
#include "Player/trainingProjCharacter.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn)	return;

	AtrainingProjCharacter* MyChar = Cast<AtrainingProjCharacter>(OwnerPawn);
	if (!MyChar)	return;

	CurrentWeapon = MyChar->CurrentWeapon;
	CurrentAction = MyChar->CurrentAction;
	//UE_LOG(LogTemp, Warning, TEXT("ToggleWeapon!! %s"), *UEnum::GetValueAsString(CurrentWeapon));
}
