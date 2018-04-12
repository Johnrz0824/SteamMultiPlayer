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
		class UTextBlock* HostUserName;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentPlayerCount;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxPlayerCount;
	UPROPERTY(meta = (BindWidget))
		class UButton* MainButton;
	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 Index);
	UFUNCTION()
	void OnServerClick();

	void SetInfo(struct FServerData info);
private:
	class UMainMenu* MainMenu;
	uint32 ButtonIndex;
};
