// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetMenuInterface(class IMenuInterface* Interface);

	void Setup();
	void Teardown();

protected:

	bool Initialize() override;
	void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld);
	
private:

	//Components
	UPROPERTY(meta = (BindWidget)) class UButton* HostButton;
	UPROPERTY(meta = (BindWidget)) class UButton* JoinButton;
	
	//Functions
	UFUNCTION() void HostPressed();
	UFUNCTION() void JoinPressed();

	class IMenuInterface* MenuInterface;
};
