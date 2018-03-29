// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
class UButton;
class UWidgetSwitcher;
class UWidget;
class UEditableTextBox;
class UScrollBox;
UCLASS()
class PUZZLEPLATFORM_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);
public:
	void SetMenuInterface(IMenuInterface* menuInterface);
	void Setup();
	void TearDown();
	void SetServerList(TArray<FString> ServerNames);
	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize()override;
	virtual void OnLevelRemovedFromWorld(ULevel * InLevel,UWorld * InWorld)override;
		
private:

		UPROPERTY(meta = (BindWidget))
		UWidget* LobbyMenu;

		UPROPERTY(meta = (BindWidget))
		UButton* HostBtn;

		UPROPERTY(meta = (BindWidget))
		UButton* JoinBtn;

		UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

		UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* MenuSwitcher;

		UPROPERTY(meta = (BindWidget))
		UWidget* JoinMenu;

		UPROPERTY(meta = (BindWidget))
		UButton* ConfirmJoinButton;

		UPROPERTY(meta = (BindWidget))
		UButton* CancelJoinButton;

		UPROPERTY(meta = (BindWidget))
		UScrollBox* ServerList;

		UFUNCTION()
		void OnHost();

		UFUNCTION()
		void OnJoin();

		UFUNCTION()
		void OnQuit();

		UFUNCTION()
		void OpenLobbyMenu();

		UFUNCTION()
		void OpenJoinMenu();


		IMenuInterface* MenuInterface;
		TSubclassOf<class UUserWidget> ServerListUnit;

		TOptional<uint32> SelectedServerIndex;
};
