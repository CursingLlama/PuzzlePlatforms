// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:

	UPuzzlePlatformsGameInstance();
	
	void Init() override;

	class UInGameMenu* GetInGameMenu() { return InGameMenu; }

	UFUNCTION(BlueprintCallable) void LoadTitleMenu();
	UFUNCTION(Exec) void Host() override;
	UFUNCTION(Exec) void Join(const FString& Address) override;
	UFUNCTION(Exec) void LoadMainMenu() override;
	UFUNCTION(Exec) void QuitGame();

private:

	UPROPERTY() TSubclassOf<class UUserWidget> TitleMenuClass;
	UPROPERTY() class UMainMenu* TitleMenu;
	UPROPERTY() TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY() class UInGameMenu* InGameMenu;
};
