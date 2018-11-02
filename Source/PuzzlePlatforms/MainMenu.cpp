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
		if (HostButton) { HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer); }
		if (JoinGameButton) { JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinGame); }
		if (MainToJoinButton) { MainToJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu); }
		if (JoinToMainButton) { JoinToMainButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu); }
		return true;
	}
	return false;
}

void UMainMenu::HostServer()
{
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	if (ServerRowClass)
	{
		ServerList->ClearChildren();

		uint32 Index = 0;
		for (const FString& Name : ServerNames)
		{
			UServerRow* ServerRow = Cast<UServerRow>(CreateWidget<UUserWidget>(this, ServerRowClass));
			if (ServerRow)
			{
				ServerRow->Setup(this, Index++);
				ServerRow->ServerName->SetText(FText::FromString(Name));
				ServerList->AddChild(ServerRow);
			}
		}
	}
}

void UMainMenu::SelectIndex(uint32 NewIndex)
{
	SelectedIndex = NewIndex;
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

