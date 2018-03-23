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
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (Actived < ActiveTriggers) return;
	if (HasAuthority())
	{
		float journeyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float journeyTravelled = (GlobalStartLocation - GetActorLocation()).Size();
		if (journeyTravelled >= journeyLength)
		{
			auto temp = GlobalTargetLocation;
			GlobalTargetLocation = GlobalStartLocation;
			GlobalStartLocation = temp;
		}
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		FVector DistanceOffset = Direction * Speed * DeltaSeconds;
		SetActorLocation(GetActorLocation() + DistanceOffset);
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	Actived++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if(Actived > 0)
		Actived--;
}