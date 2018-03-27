// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"
#include "Engine.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"
#include "OnlineSubsystem.h"
#include "UMG.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitalizer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuWidgetBPClass.Class != nullptr))return;
	MenuClass = MainMenuWidgetBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuWidgetBPClass(TEXT("/Game/MenuSystem/WBP_GameMenu"));
	if (!ensure(GameMenuWidgetBPClass.Class != nullptr))return;
	GameMenuClass = GameMenuWidgetBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("Found:%s"),*MenuClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Found:%s"), *GameMenuClass->GetName());
	OSS = IOnlineSubsystem::Get();
	if (OSS != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OSS:%s"),*OSS->GetSubsystemName().ToString());
		IOnlineSessionPtr sessionInterface = OSS->GetSessionInterface();
		if(sessionInterface.IsValid())
			UE_LOG(LogTemp, Warning, TEXT("Found Session Interface"));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No OSS!!"));

}

void UPuzzlePlatformGameInstance::Host()
{
	UEngine* engine = GetEngine();
	if (!ensure(engine != nullptr))return;

	engine->AddOnScreenDebugMessage(0,1.f,FColor::Yellow,TEXT("Hosting"));

	UWorld* world = GetWorld();
	if (!ensure(world != nullptr))return;
	world->ServerTravel("/Game/PuzzlePlatform/Lobby?listen");
}

void UPuzzlePlatformGameInstance::Join(const FString &address)
{
	UEngine* engine = GetEngine();
	if (!ensure(engine != nullptr))return;

	engine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("%s"),*(address)));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr))return;
	UE_LOG(LogTemp,Warning,TEXT("Join:%s"), *address)
	controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("Quit"));
	
	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr))return;
	controller->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass))return;
	UMainMenu* Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu))return;
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::LoadGameMenu()
{
	if (!ensure(GameMenuClass))return;
	UGameMenu* Menu = CreateWidget<UGameMenu>(this, GameMenuClass);
	if (!ensure( Menu != nullptr))return;
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::QuitGame()
{
	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr))return;
	controller->ConsoleCommand(FString("quit"), true);
}