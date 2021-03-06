// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzlePlatformGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORM_API ALobbyGameMode : public APuzzlePlatformGameMode
{
	GENERATED_BODY()

private:
	int8 players;
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting)override;
	
};
