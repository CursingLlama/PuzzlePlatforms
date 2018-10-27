// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/Dynamic/UI/MenuSystem/WBP_MainMenu"));
	if (MainMenuBPClass.Class)
	{
		MainMenuClass = MainMenuBPClass.Class;
	}

}

void UPuzzlePlatformsGameInstance::Init()
{
	if (MainMenuClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Menu class: %s"), *MainMenuClass->GetName());
	}
	
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (MainMenuClass)
	{
		UMainMenu* Menu = Cast<UMainMenu>(CreateWidget<UUserWidget>(this, MainMenuClass));
		if (Menu)
		{
			Menu->SetMenuInterface(this);
			Menu->AddToViewport();
						
			APlayerController* Controller = GetFirstLocalPlayerController();
			if (Controller)
			{
				FInputModeUIOnly InputModeData;
				InputModeData.SetWidgetToFocus(Menu->TakeWidget());
				InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

				Controller->SetInputMode(InputModeData);
				Controller->bShowMouseCursor = true;
			}
		}
	}
	
}

void UPuzzlePlatformsGameInstance::Host()
{
	APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller)
	{
		FInputModeGameOnly InputModeData;
		Controller->SetInputMode(InputModeData);
		Controller->bShowMouseCursor = false;
	}

	UEngine* Engine = GetEngine();
	if (Engine)
	{
		Engine->AddOnScreenDebugMessage(0, 4, FColor::Green, TEXT("Hosting..."));
	}
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Dynamic/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}

}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (Engine)
	{		
		Engine->AddOnScreenDebugMessage(0, 4, FColor::Green, FString::Printf(TEXT("Joining: %s"), *Address));
	}

	APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller)
	{
		Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
	
}
