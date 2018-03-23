// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenu.h"
#include "Button.h"
#include "Engine/World.h"

bool UGameMenu::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;

	CancelButton->OnClicked.AddDynamic(this, &UGameMenu::OnCancel);
	MenuButton->OnClicked.AddDynamic(this, &UGameMenu::OnQuit);

	return success;
}

void UGameMenu::SetMenuInterface(IMenuInterface* menuInterface)
{
	MenuInterface = menuInterface;
}

void UGameMenu::Setup()
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

void UGameMenu::TearDown()
{
	RemoveFromViewport();
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;
	APlayerController* controller = world->GetFirstPlayerController();
	if (!ensure(controller != nullptr)) return;

	controller->bShowMouseCursor = false;
	FInputModeGameOnly base;
	base.SetConsumeCaptureMouseDown(false);
	controller->SetInputMode(base);
}


void UGameMenu::OnQuit()
{
	if (ensure(MenuInterface))
		MenuInterface->LoadMainMenu();
}

void UGameMenu::OnCancel()
{
	TearDown();
}