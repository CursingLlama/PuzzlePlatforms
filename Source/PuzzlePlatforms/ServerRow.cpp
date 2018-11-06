// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "MainMenu.h"

#include "Components/Button.h"

void UServerRow::Setup(UMainMenu * InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
}

bool UServerRow::Initialize()
{
	if (Super::Initialize())
	{
		if (SelectButton) { SelectButton->OnClicked.AddDynamic(this, &UServerRow::OnSelectPressed); }
		
		return true;
	}
	return false;
}

void UServerRow::OnSelectPressed()
{
	if (Parent)
	{
		Parent->SelectIndex(Index);
	}	
}


