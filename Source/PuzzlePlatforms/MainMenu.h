// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

	UMainMenu();
	void SetServerList(TArray<FString> ServerNames);

	void SelectIndex(uint32 NewIndex);

protected:

	bool Initialize() override;
		
private:

	//Config
	TSubclassOf<class UUserWidget> ServerRowClass;

	//Components
	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget)) class UButton* HostButton;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinGameButton;
	UPROPERTY(meta = (BindWidget)) class UButton* MainToJoinButton;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinToMainButton;

	UPROPERTY(meta = (BindWidget)) class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget)) class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ServerList;
	
	//Functions
	UFUNCTION() void HostServer();
	UFUNCTION() void OpenJoinMenu();
	UFUNCTION() void OpenMainMenu();
	UFUNCTION() void JoinGame();

	//State
	TOptional<uint32> SelectedIndex;
};
