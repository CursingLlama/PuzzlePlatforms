// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Setup();
	void Teardown();

	void SetMenuInterface(class IMenuInterface* Interface);

	bool IsOpen() { return bIsOpen; }

protected:

	void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld);
	
	class IMenuInterface* MenuInterface;
	UPROPERTY() bool bIsOpen = false;
	
};
