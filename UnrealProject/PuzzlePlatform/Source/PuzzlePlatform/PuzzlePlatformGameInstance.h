// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "OnlineSubSystem.h"
#include "OnlineSessionInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

private:
	class IOnlineSubsystem* OSS;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	class UMainMenu* MainMenu;
public:
		UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitalizer);
		virtual void Init()override;
	
		UFUNCTION(Exec, BlueprintCallable)
			void LoadMenuWidget();
		UFUNCTION(Exec, BlueprintCallable)
			void LoadGameMenu();
		UFUNCTION(Exec)
			void OnCreateSessionCompelete(FName SessionName, bool Success);
		UFUNCTION(Exec)
			void OnDestroySessionCompelete(FName SessionName, bool Success);
		UFUNCTION(Exec)
			void OnFindSessionCompelete(bool Success);

		UFUNCTION(Exec)
			void CreateGameSession()override;
		UFUNCTION(Exec)
			void Join(const FString &address)override;

		UFUNCTION(Exec)
			void LoadMainMenu()override;

		UFUNCTION(Exec)
			void QuitGame()override;

		void RefreshServerList()override;
		TSubclassOf<class UUserWidget> MenuClass;
		TSubclassOf<class UUserWidget> GameMenuClass;
};
