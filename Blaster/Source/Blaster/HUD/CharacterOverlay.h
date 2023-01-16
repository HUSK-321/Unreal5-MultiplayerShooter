// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> HealthText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ScoreAmount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DefeatsAmount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> WeaponAmmoAmount;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CarriedAmmoAmount;
};
