// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (Super::Initialize())
	{
		if (ResumeButton) { ResumeButton->OnClicked.AddDynamic(this, &UInGameMenu::ExitMenu); }
		if (MainMenuButton) { MainMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::LeaveGame); }
		if (QuitGameButton) { QuitGameButton->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame); }

		return true;
	}
	return false;
}

void UInGameMenu::ExitMenu()
{
	Teardown();
}

void UInGameMenu::LeaveGame()
{
	if (MenuInterface)
	{
		MenuInterface->LoadMainMenu();
	}	
}

void UInGameMenu::QuitGame()
{
	if (MenuInterface)
	{
		MenuInterface->QuitGame();
	}
}
