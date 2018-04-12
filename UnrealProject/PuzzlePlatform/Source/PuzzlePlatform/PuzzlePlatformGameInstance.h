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
	class UMainMenu* MainMenu;
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
public:
		UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitalizer);
		virtual void Init()override;

		UFUNCTION(Exec, BlueprintCallable)
			void LoadMenuWidget();
		UFUNCTION(Exec, BlueprintCallable)
			void LoadGameMenu();
		UFUNCTION(Exec)
			void CreateGameSession(FText ServerName)override;
		UFUNCTION(Exec)
			void Join(uint32 Index)override;

		UFUNCTION(Exec)
			void LoadMainMenu()override;

		UFUNCTION(Exec)
			void QuitGame()override;

		void RefreshServerList()override;
		TSubclassOf<class UUserWidget> MenuClass;
		TSubclassOf<class UUserWidget> GameMenuClass;


		void OnCreateSessionCompelete(FName SessionName, bool Success);
		void OnDestroySessionCompelete(FName SessionName, bool Success);
		void OnFindSessionCompelete(bool Success);
		void OnJoinSessionCompelete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
