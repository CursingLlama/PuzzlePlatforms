// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"


void UMainMenu::SetMenuInterface(IMenuInterface * Interface)
{
	MenuInterface = Interface;
}

bool UMainMenu::Initialize()
{
	if (Super::Initialize())
	{
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostPressed);
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinPressed);
		return true;
	}
	return false;
}

void UMainMenu::HostPressed()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Join button pressed!"));
}
