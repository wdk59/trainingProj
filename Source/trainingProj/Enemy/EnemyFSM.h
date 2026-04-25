// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "IDLE"),
	Move UMETA(DisplayName = "MOVE"),
	Attack UMETA(DisplayName = "ATTACK"),
	Damage UMETA(DisplayName = "DAMAGE"),
	Die UMETA(DisplayName = "DIE"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRAININGPROJ_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	EEnemyState State;

	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	UPROPERTY()
	class AtrainingProjCharacter* Target;

	UPROPERTY()
	class AEnemyCharacter* MyCharacter;

	void SetState(EEnemyState newState);

	float CurrentTime;

	UPROPERTY(EditAnywhere)
	float AttackRange = 150.f;

	UPROPERTY(EditAnywhere)
	float FindTime = 1.f;

	UPROPERTY(EditAnywhere)
	float AttackDelayTime = 2.f;

	UPROPERTY(EditAnywhere)
	float DamageDelayTime = 2.f;

	UPROPERTY(EditAnywhere)
	float DieDelayTime = 2.f;

private:
	UPROPERTY(VisibleAnywhere)
	int HP = 2;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnDamageProcess(int damage);
};
