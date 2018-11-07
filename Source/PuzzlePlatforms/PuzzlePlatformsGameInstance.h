// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "OnlineSessionInterface.h"
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

	UFUNCTION(BlueprintCallable) class UMainMenu* GetTitleMenu() { return TitleMenu; }
	UFUNCTION(BlueprintCallable) class UInGameMenu* GetInGameMenu() { return InGameMenu; }
	UFUNCTION(BlueprintCallable) void ShowTitleMenu();
	UFUNCTION(BlueprintCallable) void ShowInGameMenu();

	//Menu Interface Functions
	UFUNCTION(Exec) void LoadTitleMenu() override;
	UFUNCTION(Exec) void Host(FString ServerName) override;
	UFUNCTION(Exec) void Join(uint32 Index) override;
	UFUNCTION(Exec) void RefreshServerList() override;
	UFUNCTION(Exec) void QuitGame() override;
	UFUNCTION() void StartSession();

private:

	UPROPERTY() TSubclassOf<class UUserWidget> TitleMenuClass;
	UPROPERTY() class UMainMenu* TitleMenu;
	UPROPERTY() TSubclassOf<class UUserWidget> InGameMenuClass;
	UPROPERTY() class UInGameMenu* InGameMenu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	FString DesiredServerName;

	void OnCreateSessionComplete(FName SessionName, bool bSucceeded);
	void OnDestroySessionComplete(FName SessionName, bool bSucceeded);
	void OnFindSessionsComplete(bool bSucceeded);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateNewSession();
};
