// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"


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
		HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostPressed);
		JoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinPressed);
		return true;
	}
	return false;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	Teardown();
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
