// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"
#include "Engine.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "UMG.h"

const static FName SESSION_NAME =TEXT("My Session Game");

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
	OSS = IOnlineSubsystem::Get();
	if (OSS != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OSS:%s"),*(OSS->GetSubsystemName().ToString()));
		SessionInterface = OSS->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionCompelete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionCompelete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionCompelete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionCompelete);
		}
	}
}

void UPuzzlePlatformGameInstance::CreateGameSession()
{
	if (SessionInterface.IsValid())
	{
		auto existingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (existingSession == nullptr)
		{
			FOnlineSessionSettings sessionSettings;
			sessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
			sessionSettings.NumPublicConnections = 2;
			sessionSettings.bShouldAdvertise = true;
			sessionSettings.bUsesPresence = true;
			SessionInterface->CreateSession(0, SESSION_NAME, sessionSettings);
		}
		else
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("No OSS!!"));
}

void UPuzzlePlatformGameInstance::OnCreateSessionCompelete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
		return;
	}
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr))return;
	world->ServerTravel("/Game/PuzzlePlatform/Lobby?listen");
}

void UPuzzlePlatformGameInstance::OnDestroySessionCompelete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Fail"));
		return;
	}
	CreateGameSession();
}

void UPuzzlePlatformGameInstance::OnJoinSessionCompelete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success && SessionInterface.IsValid())
	{
		FString address;
		if (SessionInterface->GetResolvedConnectString(SessionName, address))
		{
			APlayerController* controller = GetFirstLocalPlayerController();
			if (!ensure(controller != nullptr))return;
			controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionCompelete( bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Fail"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Find Success %d"), SessionSearch->SearchResults.Num());

	TArray<FServerData> arr;
	for (auto &result : SessionSearch->SearchResults)
	{
		FServerData data;
		data.Name = result.Session.GetSessionIdStr();
		data.HostUserName = result.Session.OwningUserId->ToString();
		data.MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
		data.MaxPlayers = result.Session.NumOpenPublicConnections;
		arr.Push(data);
	}
	MainMenu->SetServerList(arr);
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogTemp, Warning, TEXT("Find"));
	}
}

void UPuzzlePlatformGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;
	if (MainMenu != nullptr)
	{
		MainMenu->TearDown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

	/*UEngine* engine = GetEngine();
	if (!ensure(engine != nullptr))return;
	
	engine->AddOnScreenDebugMessage(0, 1.f, FColor::Yellow, FString::Printf(TEXT("%s"),*(address)));

	APlayerController* controller = GetFirstLocalPlayerController();
	if (!ensure(controller != nullptr))return;
	UE_LOG(LogTemp,Warning,TEXT("Join:%s"), *address)
	controller->ClientTravel(address, ETravelType::TRAVEL_Absolute);*/
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
	MainMenu = Menu;
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