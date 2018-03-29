// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoomUnit.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API URoomUnit : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize()override;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;
	UPROPERTY(meta = (BindWidget))
		class UButton* MainButton;

	void Setup(class UMainMenu* Parent, uint32 Index);
	void OnServerClick();
private:
	class UMainMenu* MainMenu;
	uint32 ButtonIndex;
};
