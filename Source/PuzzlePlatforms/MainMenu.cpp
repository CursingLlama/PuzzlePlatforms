// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize()
{
	if (Super::Initialize())
	{
		if (HostButton) { HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer); }
		if (JoinGameButton) { JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinGame); }
		if (MainToJoinButton) { MainToJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu); }
		if (JoinToMainButton) { JoinToMainButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu); }
		return true;
	}
	return false;
}


void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(JoinMenu);
	}	
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher && MainMenu)
	{
		MenuSwitcher->SetActiveWidget(MainMenu);
	}
}

void UMainMenu::JoinGame()
{
	if (MenuInterface && IPAddressField)
	{
		MenuInterface->Join(IPAddressField->GetText().ToString());
	}
}

