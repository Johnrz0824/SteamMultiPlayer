// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
		UPuzzlePlatformGameInstance(const FObjectInitializer & ObjectInitalizer);
		virtual void Init()override;
	
		UFUNCTION(Exec, BlueprintCallable)
			void LoadMenu();
		UFUNCTION(Exec, BlueprintCallable)
			void LoadGameMenu();
		UFUNCTION(Exec)
			void Host()override;

		UFUNCTION(Exec)
			void Join(const FString &address)override;

		UFUNCTION(Exec)
			void LoadMainMenu()override;

		UFUNCTION(Exec)
			void QuitGame()override;
		TSubclassOf<class UUserWidget> MenuClass;
		TSubclassOf<class UUserWidget> GameMenuClass;
};
