// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyFSM.h"
#include "Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/trainingProjCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	State = EEnemyState::Idle;
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	MyCharacter = Cast<AEnemyCharacter>(GetOwner());
}


void UEnemyFSM::OnDamageProcess(int damage)
{
	HP -= damage;

	if (HP > 0) {
		SetState(EEnemyState::Damage);
	}
	else {
		MyCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetState(EEnemyState::Die);
	}
}

// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case EEnemyState::Idle:
		TickIdle();
		break;
	case EEnemyState::Move:
		TickMove();
		break;
	case EEnemyState::Attack:
		TickAttack();
		break;
	case EEnemyState::Damage:
		TickDamage();
		break;
	case EEnemyState::Die:
		TickDie();
		break;
	default:
		break;
	}

	// Debug on Screen: Current State
	FString strState = UEnum::GetValueAsString(State);
	DrawDebugString(GetWorld(), MyCharacter->GetActorLocation() + FVector(0, 0, 50), strState, 0, FColor::Cyan, 0, true);
}

void UEnemyFSM::TickIdle()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;

	// Find Player -> Move
	if (CurrentTime > FindTime) {
		CurrentTime = 0;

		Target = Cast<AtrainingProjCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AtrainingProjCharacter::StaticClass()));

		if (Target) {
			SetState(EEnemyState::Move);
		}
	}
}

void UEnemyFSM::TickMove()
{
	// Look Player
	FVector direction = Target->GetActorLocation() - MyCharacter->GetActorLocation();
	MyCharacter->AddMovementInput(direction.GetSafeNormal());

	// Get Distance with Player
	float dist = direction.Size();

	// Player in Attack Range -> Attack
	if (dist <= AttackRange) {
		SetState(EEnemyState::Attack);
	}
}

void UEnemyFSM::TickAttack()
{
	CurrentTime += GetWorld()->DeltaRealTimeSeconds;

	// (Attack Delay < Waiting) -> Attack
	if (CurrentTime > AttackDelayTime) {
		UE_LOG(LogTemp, Warning, TEXT("Attack!!!!!"));

		CurrentTime = 0;
	}

	// Get Distance with Player
	float dist = MyCharacter->GetDistanceTo(Target);
	
	// (Distance > Attack Range) -> Move (to Find Player)
	if (dist > AttackRange) {
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickDamage()
{
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// (No Damaged Time > Damage Delay) -> Move
	if (DamageDelayTime < CurrentTime) {
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickDie()
{
	// Dying Motion: Go Down
	FVector P0 = MyCharacter->GetActorLocation();
	FVector vt = FVector::DownVector * 200 * GetWorld()->DeltaTimeSeconds;
	MyCharacter->SetActorLocation(P0 + vt);

	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// Destroy Myself
	if (CurrentTime > DieDelayTime) {
		MyCharacter->Destroy();
	}
}

void UEnemyFSM::SetState(EEnemyState newState)
{
	State = newState;
	CurrentTime = 0;
}
