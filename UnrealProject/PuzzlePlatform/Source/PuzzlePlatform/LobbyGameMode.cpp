// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"




void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	players++;
	UE_LOG(LogTemp,Warning,TEXT("New Player!!!  %d"),players)
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	players--;
	UE_LOG(LogTemp, Warning, TEXT("Player Gone!!!  %d"), players)
}