// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Engine/World.h"




void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	players++;
	UE_LOG(LogTemp,Warning,TEXT("New Player!!!  %d"),players)
	if (players >= 2)
	{
		UWorld* world = GetWorld();
		if (!ensure(world != nullptr))return;
		bUseSeamlessTravel = true;
		world->ServerTravel("/Game/PuzzlePlatform/Game?listen");
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	players--;
	UE_LOG(LogTemp, Warning, TEXT("Player Gone!!!  %d"), players)
}