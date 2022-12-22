// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerMenu.h"

void UMultiplayerMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	auto World{ GetWorld() };
	if(World == nullptr)	return;
	auto PlayerController{ World->GetFirstPlayerController() };
	if(PlayerController == nullptr)	return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	PlayerController->SetInputMode(InputModeData);
	PlayerController->SetShowMouseCursor(true);
	
}


