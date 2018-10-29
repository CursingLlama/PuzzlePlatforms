// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include "MenuInterface.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (Super::Initialize())
	{
		if (ResumeButton) { ResumeButton->OnClicked.AddDynamic(this, &UInGameMenu::ExitMenu); }
		
		return true;
	}
	return false;
}

void UInGameMenu::ExitMenu()
{
	Teardown();
}
