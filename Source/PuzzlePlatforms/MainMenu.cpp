// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "MenuInterface.h"
#include "ServerRow.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "UObject/ConstructorHelpers.h"

UMainMenu::UMainMenu()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Dynamic/UI/MenuSystem/WBP_ServerRow"));
	if (ServerRowBPClass.Class)
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
}

bool UMainMenu::Initialize()
{
	if (Super::Initialize())
	{
		if (HostGameButton) { HostGameButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer); }
		if (JoinGameButton) { JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinGame); }
		if (MainToJoinButton) { MainToJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu); }
		if (MainToHostButton) { MainToHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu); }
		if (JoinToMainButton) { JoinToMainButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu); }
		if (HostToMainButton) { HostToMainButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu); }
		return true;
	}
	return false;
}

void UMainMenu::HostServer()
{
	if (MenuInterface && SessionName)
	{
		MenuInterface->Host(FName(*SessionName->GetText().ToString()));
	}
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	if (ServerRowClass)
	{
		ServerList->ClearChildren();

		uint32 Index = 0;
		for (const FServerData& Data : ServerNames)
		{
			UServerRow* ServerRow = Cast<UServerRow>(CreateWidget<UUserWidget>(this, ServerRowClass));
			if (ServerRow)
			{
				ServerRow->Setup(this, Index++);
				ServerRow->ServerName->SetText(FText::FromString(Data.SessionName));
				ServerRow->PlayerCount->SetText(FText::FromString(FString::Printf(TEXT("%i/%i"), Data.CurrentPlayers, Data.MaxPlayers)));
				ServerRow->Ping->SetText(FText::FromString(FString::Printf(TEXT("%i ms"), Data.Ping)));
				ServerList->AddChild(ServerRow);
			}
		}
	}
}

void UMainMenu::SelectIndex(uint32 NewIndex)
{
	if (!ServerList) return;

	SelectedIndex = NewIndex;
	for (int i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		UServerRow* Child = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Child && SelectedIndex.IsSet())
		{
			Child->bIsSelected = (i == SelectedIndex.GetValue());
		}
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(JoinMenu);
		if (MenuInterface)
		{
			MenuInterface->RefreshServerList();
		}		
	}	
}

void UMainMenu::OpenHostMenu()
{
	if (MenuSwitcher && HostMenu)
	{
		MenuSwitcher->SetActiveWidget(HostMenu);		
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
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		MenuInterface->Join(SelectedIndex.GetValue());
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex: %d"), SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex not set."));
	}	
}

