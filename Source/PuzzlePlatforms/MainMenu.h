// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"


USTRUCT()
struct FServerData
{
	GENERATED_USTRUCT_BODY()

	FString SessionID;
	FString SessionName;
	FString HostUsername;
	
	int32 Ping;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;	
};

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:

	UMainMenu();
	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 NewIndex);

protected:

	bool Initialize() override;
		
private:

	//Config
	TSubclassOf<class UUserWidget> ServerRowClass;

	//Components
	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget)) class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget)) class UButton* MainToJoinButton;
	UPROPERTY(meta = (BindWidget)) class UButton* MainToHostButton;

	UPROPERTY(meta = (BindWidget)) class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget)) class UPanelWidget* ServerList;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinToMainButton;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget)) class UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget)) class UButton* HostToMainButton;
	UPROPERTY(meta = (BindWidget)) class UButton* HostGameButton;
	UPROPERTY(meta = (BindWidget)) class UEditableTextBox* SessionName;
	
	//Functions
	UFUNCTION() void HostServer();
	UFUNCTION() void OpenJoinMenu();
	UFUNCTION() void OpenHostMenu();
	UFUNCTION() void OpenMainMenu();
	UFUNCTION() void JoinGame();

	//State
	TOptional<uint32> SelectedIndex;
};
