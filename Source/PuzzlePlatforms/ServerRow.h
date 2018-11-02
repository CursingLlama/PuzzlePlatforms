// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
		
public:

	void Setup(class UMainMenu* InMenu, uint32 InIndex);

	UPROPERTY(BlueprintReadOnly) bool bIsSelected = false;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ServerName;
	
protected:

	bool Initialize() override;

private:

	//State
	UPROPERTY() class UMainMenu* Parent;
	UPROPERTY() uint32 Index;

	//Components
	UPROPERTY(meta = (BindWidget)) class UButton* SelectButton;

	//Functions
	UFUNCTION() void OnSelectPressed();

};
