// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include <string>

#include "Tank.h"
#include "ToonTanksPlayerController.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	UKismetSystemLibrary::PrintString(this, "Tower Tank Alive: " + FString::FromInt(TargetTowers));
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();
	
	if (ToonTanksPlayerController)
	{
		ToonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
											ToonTanksPlayerController,
											&AToonTanksPlayerController::SetPlayerEnabledState, 
											true);
		// true -> input parameter of "&AToonTanksPlayerController::SetPlayerEnabledState"

		GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
										PlayerEnableTimerDelegate,
										StartDelay,
										false);
	}
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController)
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
		// return;
	}

	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		TargetTowers--;
		UKismetSystemLibrary::PrintString(this, "Tower Tank Alive: " + FString::FromInt(TargetTowers));
		if (TargetTowers == 0) GameOver(true);
	}
	// ATower* DestroyedTower = Cast<ATower>(DeadActor);
	// DestroyedTower->HandleDestruction();
}

int AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
	return Towers.Num();
}
