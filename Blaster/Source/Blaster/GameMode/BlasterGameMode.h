// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	 extern BLASTER_API const FName Cooldown;   // Match duration has been reached. Display winner and begin cooldown timer;
}

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ABlasterGameMode();

	virtual void Tick(float DeltaTime) override;

	virtual void PlayerEliminated(class ABlasterCharacter* ElimmedCharacter, class ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);

	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, class AController* ElimmedController);

	UPROPERTY(EditDefaultsOnly)
	float WarmupTime;

	UPROPERTY(EditDefaultsOnly)
	float MatchTime;

	UPROPERTY(EditDefaultsOnly)
	float CooldownTime;

	float LevelStartingTime;

protected:

	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;

private:
	
	float CountdownTime;
};
