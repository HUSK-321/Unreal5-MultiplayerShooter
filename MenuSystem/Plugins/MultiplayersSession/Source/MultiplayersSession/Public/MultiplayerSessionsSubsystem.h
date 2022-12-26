// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

/**
 * 
 */
 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);
 
UCLASS()
class MULTIPLAYERSSESSION_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	UMultiplayerSessionsSubsystem();

	/** To handle session functionally, thr menu class will call these */
	void CreateSession(int32 NumPublicConnections, FString MatchType);

	void FindSession(int32 MaxSearchResults);

	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	void DestroySession();

	void StartSession();

	/** Callback delegate for menu system */
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;

	FMultiplayerOnFindSessionComplete MultiplayerOnFindSessionComplete;

	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;

	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;

	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
	
	
protected:

	/*
	 * Internal callbacks will add to the Online Session Interface delegate list
	 * Don't need to be called outside this class
	 */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	

private:
	
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSettings> LastSessinoSettings;

	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	/** To add to the online session interface delegate list
	 *  Will bind our MultiplayerSessionsSubsystem internal callbacks to these
	 */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessioncompleteDelegateHandle;

	bool bCreateSessionOnDestroy{ false };

	int32 LastNumPublicConnection;

	FString LastMatchType;
};
