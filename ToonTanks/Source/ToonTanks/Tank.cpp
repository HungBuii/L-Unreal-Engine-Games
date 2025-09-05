// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());

	// DrawDebugSphere(
	// 	GetWorld(),
	// 	GetActorLocation() + FVector(0.f, 0.f, 200.f),
	// 	100.f,
	// 	12,
	// 	FColor::Red,
	// 	false,
	// 	30.f);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult);
			
		// DrawDebugSphere(
  //       		GetWorld(),
  //       		HitResult.ImpactPoint,
  //       		25.f,
  //       		12,
  //       		FColor::Red,
  //       		false,
  //       		-1.f);

		RotateTurret(HitResult.ImpactPoint);
        		
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis Mapping
	PlayerInputComponent->BindAxis("MoveForward", this, &ATank::Move); 
	PlayerInputComponent->BindAxis("Turn", this, &ATank::Turn);

	// Action Mapping
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);
}

// Call every single frame with function "BindAxis" (a delegate function an Axis)
void ATank::Move(float Value)
{
	// UE_LOG(LogTemp, Warning, TEXT("%f"), Value);
	FVector DeltaLocation = FVector::ZeroVector;
	// X = Value * Speed * DeltaTime
	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	FRotator DeltaRotation = FRotator::ZeroRotator;
	// Yaw = Value * TurnRate * DeltaTime
	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	bAlive = false;
	
}