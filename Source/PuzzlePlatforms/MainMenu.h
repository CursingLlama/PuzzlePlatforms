// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetMenuInterface(class IMenuInterface* Interface);

	void Setup();
	void Teardown();

protected:

	bool Initialize() override;
	void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld);
	
private:

	//Components
	UPROPERTY(meta = (BindWidget)) class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget)) class UButton* HostButton;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget)) class UButton* MainToJoinButton;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinToMainButton;

	UPROPERTY(meta = (BindWidget)) class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget)) class UWidget* MainMenu;

	
	//Functions
	UFUNCTION() void HostServer();
	UFUNCTION() void OpenJoinMenu();
	UFUNCTION() void OpenMainMenu();

	class IMenuInterface* MenuInterface;
};
