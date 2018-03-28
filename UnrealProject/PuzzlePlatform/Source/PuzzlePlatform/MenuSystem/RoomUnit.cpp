// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomUnit.h"




void URoomUnit::Setup()
{
	AddToViewport();

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;
	APlayerController* controller = world->GetFirstPlayerController();
	if (!ensure(controller != nullptr)) return;

	FInputModeUIOnly base;
	base.SetWidgetToFocus(TakeWidget());
	base.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	controller->SetInputMode(base);
	controller->bShowMouseCursor = true;
}