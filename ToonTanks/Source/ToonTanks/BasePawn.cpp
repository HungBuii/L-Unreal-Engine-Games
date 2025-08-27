// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Projectitle.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectitleSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectitleSpawnPoint->SetupAttachment(TurretMesh);
}

// Called every frame
// void ABasePawn::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	// FVector DeltaLocation(0.f);
// 	// DeltaLocation.X = 2.f;
// 	// AddActorLocalOffset(DeltaLocation);
// }

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	// All vectors using are "World Space" not "Local Space"
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
	// FRotator LookAtRotation = ToTarget.Rotation(); // Not use it because we will take care only "Yaw" rotation
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	
	TurretMesh->SetWorldRotation // Not use "AddActorLocalRotation()"
	(
		FMath::RInterpTo(TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			25.f) // Interpolation speed, if the speed given is 0, then jump to the target. Speed higher -> spin faster 
			); // Use "Interpolation" help tries to reach Target rotation based on Current rotation,
	           // giving a nice smooth feeling when rotating to Target rotation. (Target is Kismet Math Library)
	
}

void ABasePawn::Fire()
{
	// FVector ProjectileSpawnPointLocation = ProjectitleSpawnPoint->GetComponentLocation();
	// DrawDebugSphere(
	// 	GetWorld(),
	// 	ProjectileSpawnPointLocation,
	// 	25.f,
	// 	12,
	// 	FColor::Green,
	// 	false,
	// 	3.f);

	FVector Location = ProjectitleSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectitleSpawnPoint->GetComponentRotation();

	auto Projectile = GetWorld()->SpawnActor<AProjectitle>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(this);
}

void ABasePawn::HandleDestruction()
{
	/* TODO: visual/sound effects */
	
}


