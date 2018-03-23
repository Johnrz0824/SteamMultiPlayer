// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* menuInterface);
	void Setup();
	void TearDown();

protected:
		virtual bool Initialize()override;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		UButton* MenuButton;

	UFUNCTION()
		void OnCancel();

	UFUNCTION()
		void OnQuit();

	class IMenuInterface* MenuInterface;
};
