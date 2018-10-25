// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:

	AMovingPlatform();

	void BeginPlay() override;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

protected:
	
	void Tick(float DeltaTime) override;

private:

	//Config
	UPROPERTY(EditAnywhere) float MoveSpeed = 100;
	UPROPERTY(EditAnywhere) int32 RequiredTriggers = 0;
	
	//Components
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true)) FVector TargetLocation;

	//State
	UPROPERTY() FVector OriginLocation;
	UPROPERTY() FVector Direction = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere) int32 ActiveTriggers = 0;

};
