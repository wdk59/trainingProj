// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BulletActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Enemy/EnemyCharacter.h"

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Collision
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAll"));
	RootComponent = CollisionComp;

	// Mesh
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
	MeshComp->SetupAttachment(RootComponent);

	// Movement
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(CollisionComp);

	MovementComp->InitialSpeed = 5000.f;
	MovementComp->MaxSpeed = 5000.f;

	MovementComp->bShouldBounce = true;
	MovementComp->Bounciness = 0.3f;

	SetLifeSpan(3.f);
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABulletActor::OnOverlapBegin);
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("hit"));
	OnProjectileHit(OtherActor);
}

void ABulletActor::OnProjectileHit(AActor* hitActor)
{
	if (hitActor == GetOwner())
		return;

	if (Cast<AEnemyCharacter>(hitActor)) {
		Cast<AEnemyCharacter>(hitActor)->OnTakeDamage(damage);
	}
}
