// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


void UMainMenu::SetMenuInterface(IMenuInterface * Interface)
{
	MenuInterface = Interface;
}

void UMainMenu::Setup()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			Controller->SetInputMode(InputModeData);
			Controller->bShowMouseCursor = true;
		}
	}	
	AddToViewport();
}

void UMainMenu::Teardown()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			FInputModeGameOnly InputModeData;
			Controller->SetInputMode(InputModeData);
			Controller->bShowMouseCursor = false;
		}
	}
	RemoveFromViewport();
}

bool UMainMenu::Initialize()
{
	if (Super::Initialize())
	{
		if (HostButton) { HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer); }
		if (MainToJoinButton) { MainToJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu); }
		if (JoinToMainButton) { JoinToMainButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu); }
		return true;
	}
	return false;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Teardown();
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


