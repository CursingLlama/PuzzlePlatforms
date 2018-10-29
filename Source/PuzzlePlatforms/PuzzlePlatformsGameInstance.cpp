// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "InGameMenu.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance()
{
	ConstructorHelpers::FClassFinder<UUserWidget> TitleMenuBPClass(TEXT("/Game/Dynamic/UI/MenuSystem/WBP_MainMenu"));
	if (TitleMenuBPClass.Class)
	{
		TitleMenuClass = TitleMenuBPClass.Class;
	}
	
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/Dynamic/UI/MenuSystem/WBP_InGameMenu"));
	if (InGameMenuBPClass.Class)
	{
		InGameMenuClass = InGameMenuBPClass.Class;		
	}
}

void UPuzzlePlatformsGameInstance::Init()
{
	if (InGameMenuClass)
	{
		InGameMenu = Cast<UInGameMenu>(CreateWidget<UUserWidget>(this, InGameMenuClass));
	}
}

void UPuzzlePlatformsGameInstance::LoadTitleMenu()
{
	if (TitleMenuClass)
	{
		TitleMenu = Cast<UMainMenu>(CreateWidget<UUserWidget>(this, TitleMenuClass));
		if (TitleMenu)
		{
			TitleMenu->SetMenuInterface(this);
			TitleMenu->Setup();
		}
	}	
}

void UPuzzlePlatformsGameInstance::Host()
{
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
