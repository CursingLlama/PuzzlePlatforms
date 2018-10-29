// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:

	bool Initialize() override;
	
private:

	//Components
	UPROPERTY(meta = (BindWidget)) class UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget)) class UButton* MainMenuButton;
	UPROPERTY(meta = (BindWidget)) class UButton* QuitGameButton;
	
	//Functions
	UFUNCTION() void ExitMenu();
};
