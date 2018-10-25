// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API APlatformTrigger : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	APlatformTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//Config
	UPROPERTY(EditAnywhere) TArray<class AMovingPlatform*> Platforms;
	
	//Components
	UPROPERTY(VisibleAnywhere) class UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere) class UBoxComponent* TriggerVolume;

	//Config
	UPROPERTY(EditAnywhere) float DepressAmount = 5;

	//State
	int32 OverlappingActors = 0;

	//Functions
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION() void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
