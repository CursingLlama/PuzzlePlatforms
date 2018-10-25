// Fill out your copyright notice in the Description page of Project Settings.

#include "PlatformTrigger.h"
#include "MovingPlatform.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(FName("Root")));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Trigger Plate Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("Trigger Volume"));
	if (TriggerVolume)
	{
		TriggerVolume->SetupAttachment(GetRootComponent());
	}
	
	
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerVolume)
	{
		TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
		TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
	}
}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OverlappingActors++ == 0)
	{
		Mesh->AddRelativeLocation(FVector(0, 0, -DepressAmount));
	}

	for (AMovingPlatform* Platform : Platforms)
	{
		if (Platform)
		{
			Platform->AddActiveTrigger();
		}
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (--OverlappingActors == 0)
	{
		Mesh->AddRelativeLocation(FVector(0, 0, DepressAmount));
	}
	
	for (AMovingPlatform* Platform : Platforms)
	{
		if (Platform)
		{
			Platform->RemoveActiveTrigger();
		}
	}
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

