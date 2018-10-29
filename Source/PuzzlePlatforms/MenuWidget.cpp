// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"
#include "MenuInterface.h"

void UMenuWidget::Setup()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller)
		{
			FInputModeGameAndUI InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
						
			Controller->SetInputMode(InputModeData);
			Controller->bShowMouseCursor = true;
		}
	}
	AddToViewport();
	bIsOpen = true;
}

void UMenuWidget::Teardown()
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
	bIsOpen = false;
}

void UMenuWidget::SetMenuInterface(IMenuInterface * Interface)
{
	MenuInterface = Interface;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	if (IsOpen())
	{
		Teardown();
	}	
}
