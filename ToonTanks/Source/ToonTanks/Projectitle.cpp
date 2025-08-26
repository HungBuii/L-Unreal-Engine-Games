// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectitle.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AProjectitle::AProjectitle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectitleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectitleMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f; 
}

// Called when the game starts or when spawned
void AProjectitle::BeginPlay()
{
	Super::BeginPlay();

	ProjectitleMesh->OnComponentHit.AddDynamic(this, &AProjectitle::OnHit);
}

// Called every frame
void AProjectitle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectitle::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	// UE_LOG(LogTemp, Warning, TEXT("HitComp: %s"), *HitComp->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *OtherComp->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("NormalImpulse: %s"), *NormalImpulse.ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *Hit.ToString());

	auto MyOwner = GetOwner();
	if (MyOwner == nullptr) return;

	auto MyOwnerInstigator = MyOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor != nullptr && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		Destroy();
	}
}

