// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if(GameState == nullptr)	return;
	
	PrintNumberOfPlayerLog(false);
	if(GEngine)
	{
		auto PlayerState{ NewPlayer->GetPlayerState<APlayerState>() };
		if(PlayerState)
		{
			auto PlayerName{ PlayerState->GetPlayerName() };
			GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s joined the Game"), *PlayerName));
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PrintNumberOfPlayerLog(true);
	
	auto PlayerState{ Exiting->GetPlayerState<APlayerState>() };
	if(PlayerState)
	{
		auto PlayerName{ PlayerState->GetPlayerName() };
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Cyan, FString::Printf(TEXT("%s left the Game"), *PlayerName));
	}
	
}

void ALobbyGameMode::PrintNumberOfPlayerLog(bool bIsLogOut)
{
	if(GEngine == nullptr)	return;
	
	auto NumberOfPlayer{ GameState.Get()->PlayerArray.Num() };
	if(bIsLogOut)
	{
		NumberOfPlayer--;
	}
	GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Yellow, FString::Printf(TEXT("Players in Game : %d"), NumberOfPlayer));
}
