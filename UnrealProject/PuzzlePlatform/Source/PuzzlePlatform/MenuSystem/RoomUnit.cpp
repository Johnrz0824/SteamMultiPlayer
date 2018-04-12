// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomUnit.h"
#include "Button.h"
#include "TextBlock.h"
#include "MenuSystem/MainMenu.h"



void URoomUnit::Setup(class UMainMenu* Parent, uint32 Index)
{
	MainMenu = Parent;
	ButtonIndex = Index;
}
bool URoomUnit::Initialize()
{
	bool success = Super::Initialize();
	if (!success) return false;
	MainButton->OnClicked.AddDynamic(this, &URoomUnit::OnServerClick);

	return success;
}

void URoomUnit::OnServerClick()
{
	MainMenu->SelectIndex(ButtonIndex);
}

void URoomUnit::SetInfo(struct FServerData info)
{
	ServerName->SetText(FText::FromString(info.Name));
	HostUserName->SetText(FText::FromString(info.HostUserName));
	CurrentPlayerCount->SetText(FText::FromString(FString::FromInt(info.CurrentPlayers)));
	MaxPlayerCount->SetText(FText::FromString(FString::FromInt(info.MaxPlayers)));
}