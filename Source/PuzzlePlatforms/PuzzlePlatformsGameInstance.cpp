// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "PlatformTrigger.h"
#include "MainMenu.h"
#include "InGameMenu.h"
#include "PuzzlePlatformsGameMode.h"

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
		InGameMenu->SetMenuInterface(this);
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
	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Dynamic/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller)
	{
		Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller->HasAuthority())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			APuzzlePlatformsGameMode* GameMode = World->GetAuthGameMode<APuzzlePlatformsGameMode>();
			if (GameMode)
			{
				GameMode->ReturnToMainMenuHost();
			}
		}		
	}
	else
	{
		Controller->ClientReturnToMainMenuWithTextReason(FText::FromString("Leaving game..."));
	}	
}

void UPuzzlePlatformsGameInstance::QuitGame()
{
	APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller)
	{
		Controller->ConsoleCommand("Quit");
	}
}
