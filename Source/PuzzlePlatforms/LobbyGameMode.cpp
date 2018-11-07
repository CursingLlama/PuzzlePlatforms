// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "PuzzlePlatformsGameInstance.h"

#include "Engine/World.h"
#include "TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (++PlayerCount >= StartingPlayerCount)
	{
		GetWorldTimerManager().ClearTimer(StartTimer);
		GetWorldTimerManager().SetTimer(StartTimer, this, &ALobbyGameMode::StartSession, BufferTime, false);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (--PlayerCount < StartingPlayerCount)
	{
		GetWorldTimerManager().ClearTimer(StartTimer);
	}
}

void ALobbyGameMode::StartSession()
{
	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->StartSession();
	}

	bUseSeamlessTravel = true;
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Dynamic/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}
