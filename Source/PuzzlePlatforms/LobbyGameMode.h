// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformsGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API ALobbyGameMode : public APuzzlePlatformsGameMode
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;

private:

	UFUNCTION() void StartSession();

	UPROPERTY() uint32 PlayerCount = 0;
	UPROPERTY() uint32 StartingPlayerCount = 2;
	UPROPERTY() FTimerHandle StartTimer;
	UPROPERTY() float BufferTime = 10;
	
};
