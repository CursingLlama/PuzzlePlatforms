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

protected:
	
	void Tick(float DeltaTime) override;

private:

	//State
	UPROPERTY(EditAnywhere) float MoveSpeed = 100;
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true)) FVector TargetLocation;
	UPROPERTY() FVector OriginLocation;
	UPROPERTY() FVector Direction = FVector::ZeroVector;

};
