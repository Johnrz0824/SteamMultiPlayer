// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuSystem/RoomUnit.h"
#include "Blueprint/UserWidget.h"
#include "EditableTextBox.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer):Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerListUnitBPClass(TEXT("/Game/MenuSystem/WBP_RoomUnit"));
	if (!ensure(ServerListUnitBPClass.Class != nullptr))return;
	ServerListUnit = ServerListUnitBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool success =	Super::Initialize();
	if (!success) return false;

	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::OnQuit);
	HostBtn->OnClicked.AddDynamic(this,&UMainMenu::OnHost);
	JoinBtn->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	ConfirmJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoin);
	CancelJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenLobbyMenu);

	return success;
}
void UMainMenu::SetMenuInterface(IMenuInterface* menuInterface)
{
	MenuInterface = menuInterface;
}

void UMainMenu::OnHost()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->CreateGameSession();
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return;
	if (!ensure(JoinMenu != nullptr))return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	MenuInterface->RefreshServerList();
}

void UMainMenu::OpenLobbyMenu()
{
	if (!ensure(MenuSwitcher != nullptr))return;
	if (!ensure(LobbyMenu != nullptr))return;
	MenuSwitcher->SetActiveWidget(LobbyMenu);
}

void UMainMenu::OnJoin()
{
	if (SelectedServerIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected:%d"), SelectedServerIndex.GetValue());
		MenuInterface->Join(SelectedServerIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Selected"));
	}
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	ServerList->ClearChildren();
	uint32 index = 0;
	if (MenuInterface != nullptr)
	{
		if (!ensure(ServerListUnit))return;
		for (const auto &name : ServerNames)
		{
			URoomUnit* unit = CreateWidget<URoomUnit>(GetWorld()->GetGameInstance(), ServerListUnit);
			if (!ensure(unit))return;
			ServerList->AddChild(unit);
			unit->Setup(this, index);
			unit->ServerName->SetText(FText::FromString(name));
			index++;
		}
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedServerIndex = Index;
}

void UMainMenu::OnQuit()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->QuitGame();
	}
}

void UMainMenu::Setup()
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

void UMainMenu::TearDown()
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

void UMainMenu::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	Super::OnLevelRemovedFromWorld(InLevel,InWorld);
	TearDown();
}