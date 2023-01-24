// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	void SetHUDHealth(float Health, float MaxHealth);

	void SetHUDScore(float Score);

	void SetHUDDefeats(int32 Defeats);

	void SetHUDWeaponAmmo(int32 Ammo);

	void SetHUDCarriedAmmo(int32 Ammo);

	void SetHUDMatchCountdown(float CountdownTime);

	void SetHUDAnnouncementCountdown(float CountdownTime);

	virtual void OnPossess(APawn* InPawn) override;

	virtual float GetServerTime();

	virtual void ReceivedPlayer() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnMatchStateSet(FName State);
	
	void HandleMatchHasStarted();

	void HandleCooldown();

protected:

	virtual void BeginPlay() override;
	void CheckTimeSync(float DeltaTime);
	bool CharacterOverlayHUDIsValid();

	void SetHUDTime();
	
	void PoolInit();

	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequeset);

	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.0f;

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidGame(FName StateOfMatch, float Warmup, float Match, float StartingTime);

private:

	UPROPERTY()
	TObjectPtr<class ABlasterHUD> BlasterHUD;
	
	float MatchTime = 0.f;

	float WarmupTime = 0.f;

	float LevelStartingTime = 0.f;
	
	uint32 CountdownInt = 0;
	
	float ClientServerDelta = 0.f; // difference btw ser and client time

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	TObjectPtr<class UCharacterOverlay> CharacterOverlay;

	bool bInitializeCharacterOverlay = false;

	float HUDHealth;
	float HUDMaxHealth;
	float HUDScore;
	int32 HUDDefeats;
};
