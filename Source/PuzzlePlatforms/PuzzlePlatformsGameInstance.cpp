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

static const FName NAME_KEY("GameSession");

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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
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

void UPuzzlePlatformsGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;
	if (SessionInterface)
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
		if (ExistingSession)
		{
			SessionInterface->DestroySession(NAME_GameSession);
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
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL";
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(NAME_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
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
		World->ServerTravel("/Game/Dynamic/ThirdPersonCPP/Maps/Lobby?listen");
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

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (SessionInterface && SessionSearch && SessionSearch->SearchResults.IsValidIndex(Index))
	{
		SessionInterface->JoinSession(0, NAME_GameSession, SessionSearch->SearchResults[Index]);
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	APlayerController* Controller = GetFirstLocalPlayerController();
	if (Controller && SessionInterface)
	{
		FString URL;
		if (SessionInterface->GetResolvedConnectString(SessionName, URL))
		{
			UEngine* Engine = GetEngine();
			if (Engine)
			{
				Engine->AddOnScreenDebugMessage(0, 4, FColor::Green, FString::Printf(TEXT("Joining: %s"), *URL));
			}
			Controller->ClientTravel(URL, ETravelType::TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[Join Delegate] Could not get resolved string!"));
		}
	}
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finding Sessions..."));

		//SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bSucceeded)
{
	if (SessionSearch && bSucceeded)
	{
		if (SessionSearch->SearchResults.Num() == 0) UE_LOG(LogTemp, Warning, TEXT("No open sessions found!"));
		TArray<FServerData> Servers;
		
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			FServerData ServerData;

			ServerData.SessionID = Result.GetSessionIdStr();
			
			FString Name;
			if (Result.Session.SessionSettings.Get(NAME_KEY, Name))
			{
				ServerData.SessionName = Name;
			}
			else
			{
				ServerData.SessionName = ServerData.SessionID;
			}
			
			ServerData.Ping = Result.PingInMs;
			ServerData.HostUsername = Result.Session.OwningUserName;
			ServerData.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = ServerData.MaxPlayers - Result.Session.NumOpenPublicConnections;
			
			Servers.Emplace(ServerData);
			UE_LOG(LogTemp, Warning, TEXT("Server=[%s] HostName=[%s] Ping=[%i] Players=[%i/%i]"), *ServerData.SessionID, *ServerData.HostUsername, ServerData.Ping, ServerData.CurrentPlayers, ServerData.MaxPlayers);
		}
		if (TitleMenu)
		{
			TitleMenu->SetServerList(Servers);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FindSessions failed"));
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

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface)
	{
		SessionInterface->StartSession(NAME_GameSession);
	}
}
