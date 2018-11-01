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
#include "OnlineSubsystem.h"

#include "OnlineSessionSettings.h"

static const FName SESSION_NAME("GameSession");

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
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface)
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);		
		}
	}

	if (InGameMenuClass)
	{
		InGameMenu = Cast<UInGameMenu>(CreateWidget<UUserWidget>(this, InGameMenuClass));
		if (InGameMenu)
		{
			InGameMenu->SetMenuInterface(this);
		}		
	}

	if (TitleMenuClass)
	{
		TitleMenu = Cast<UMainMenu>(CreateWidget<UUserWidget>(this, TitleMenuClass));
		if (TitleMenu)
		{
			TitleMenu->SetMenuInterface(this);
		}
	}
}

void UPuzzlePlatformsGameInstance::ShowTitleMenu()
{
	TitleMenu->Setup();
}

void UPuzzlePlatformsGameInstance::ShowInGameMenu()
{
	InGameMenu->Setup();
}

void UPuzzlePlatformsGameInstance::LoadTitleMenu()
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

void UPuzzlePlatformsGameInstance::Host()
{
	if (SessionInterface)
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateNewSession();
		}		
	}	
}

void UPuzzlePlatformsGameInstance::CreateNewSession()
{
	if (SessionInterface)
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, FName(SESSION_NAME), SessionSettings);
	}	
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bSucceeded)
{
	if (!bSucceeded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create [%s]"), *SessionName.ToString());
		return;
	}

	UWorld* World = GetWorld();
	if (World)
	{
		World->ServerTravel("/Game/Dynamic/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bSucceeded)
{
	if (bSucceeded)
	{
		CreateNewSession();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy [%s]"), *SessionName.ToString());
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bSucceeded)
{
	if (SessionSearch && bSucceeded)
	{
		if (SessionSearch->SearchResults.Num() == 0) UE_LOG(LogTemp, Warning, TEXT("No open sessions found!"));
		TArray<FString> ServerNames;
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			ServerNames.Emplace(Result.GetSessionIdStr());
		}
		if (TitleMenu)
		{
			TitleMenu->SetServerList(ServerNames);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FindSessions failed"));
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	/*APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller)
	{
		Controller->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}*/
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finding Sessions..."));
		SessionSearch->bIsLanQuery = true;

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
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
