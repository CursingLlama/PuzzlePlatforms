// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"



AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);		
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	OriginLocation = GetActorLocation();
	TargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}	
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && ActiveTriggers >= RequiredTriggers)
	{
		if (FVector::PointsAreNear(GetActorLocation(), OriginLocation, MoveSpeed * DeltaTime))
			Direction = (TargetLocation - OriginLocation).GetSafeNormal();
		if (FVector::PointsAreNear(GetActorLocation(), TargetLocation, MoveSpeed * DeltaTime))
			Direction = (OriginLocation - TargetLocation).GetSafeNormal();
		
		FVector Location = GetActorLocation();
		Location += Direction * MoveSpeed * DeltaTime;
		SetActorLocation(Location);
	}

}
