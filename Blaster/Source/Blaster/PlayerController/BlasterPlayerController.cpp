// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/GameMode/BlasterGameMode.h"


void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
}

void ABlasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetHUDTime();

	CheckTimeSync(DeltaTime);

	PoolInit();
}

void ABlasterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerController, MatchState);
}

void ABlasterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto BlasterCharacter = Cast<ABlasterCharacter>(InPawn);
	if(BlasterCharacter)
	{
		SetHUDHealth(BlasterCharacter->GetHealth(), BlasterCharacter->GetMaxHealth());
	}
}

bool ABlasterPlayerController::HUDIsValid()
{
	return (BlasterHUD && BlasterHUD->CharacterOverlay);
}

void ABlasterPlayerController::SetHUDTime()
{
	uint32 SecondsLeft = FMath::CeilToInt(MatchTime - GetServerTime());
	if (CountdownInt != SecondsLeft)
	{
		SetHUDMatchCountdown(MatchTime - GetServerTime());
	}

	CountdownInt = SecondsLeft;
}

void ABlasterPlayerController::PoolInit()
{
	if(CharacterOverlay != nullptr) return;
	
	if(BlasterHUD && BlasterHUD->CharacterOverlay)
	{
		CharacterOverlay = BlasterHUD->CharacterOverlay;
		if(CharacterOverlay)
		{
			SetHUDHealth(HUDHealth, HUDMaxHealth);
			SetHUDScore(HUDScore);
			SetHUDDefeats(HUDDefeats);
		}
	}
}

void ABlasterPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!HUDIsValid() || BlasterHUD->CharacterOverlay->HealthBar == nullptr || BlasterHUD->CharacterOverlay->HealthText == nullptr)
	{
		bInitializeCharacterOverlay = true;
		HUDHealth = Health;
		HUDMaxHealth = MaxHealth;
		return;
	}

	const float HealthPercent = Health / MaxHealth;
	BlasterHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
	FString HealthText = FString::Printf(TEXT("%d / %d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
	BlasterHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
}

void ABlasterPlayerController::SetHUDScore(float Score)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!HUDIsValid() || BlasterHUD->CharacterOverlay->ScoreAmount == nullptr)
	{
		bInitializeCharacterOverlay = true;
		HUDScore = Score;
		return;
	}

	FString PlayerScore = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
	BlasterHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(PlayerScore));
}

void ABlasterPlayerController::SetHUDDefeats(int32 Defeats)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!HUDIsValid() || BlasterHUD->CharacterOverlay->DefeatsAmount == nullptr)
	{
		bInitializeCharacterOverlay = true;
		HUDDefeats = Defeats;
		return;
	}

	FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
	BlasterHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
}

void ABlasterPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!HUDIsValid() || BlasterHUD->CharacterOverlay->WeaponAmmoAmount == nullptr)	return;

	FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
	BlasterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
}

void ABlasterPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!HUDIsValid() || BlasterHUD->CharacterOverlay->CarriedAmmoAmount == nullptr)	return;

	FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
	BlasterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
}

void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!HUDIsValid() || BlasterHUD->CharacterOverlay->MatchCountdownText == nullptr)	return;

	int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
	int32 Seconds = FMath::FloorToInt(CountdownTime - Minutes * 60.f);
	FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
	BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
}

void ABlasterPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequeset)
{
	float ServerTimeOfreceipt = GetWorld()->GetTimeSeconds();
	ClientReportServerTime(TimeOfClientRequeset, ServerTimeOfreceipt);
}

void ABlasterPlayerController::ClientReportServerTime_Implementation(float TimeOfClientRequest,
	float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (0.5f * RoundTripTime);
	ClientServerDelta = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

float ABlasterPlayerController::GetServerTime()
{
	return GetWorld()->GetTimeSeconds() + ClientServerDelta;
}

void ABlasterPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if(IsLocalController())
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
	}
}

void ABlasterPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;
	if(IsLocalController() && TimeSyncRunningTime > TimeSyncFrequency)
	{
		ServerRequestServerTime(GetWorld()->GetTimeSeconds());
		TimeSyncRunningTime = 0.f;
	}
}

void ABlasterPlayerController::OnMatchStateSet(FName State)
{
	MatchState = State;

	if(MatchState == MatchState::InProgress)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if(BlasterHUD == nullptr)	return;

		BlasterHUD->AddCharacterOverlay();
	}
}

void ABlasterPlayerController::OnRep_MatchState()
{
	if(MatchState == MatchState::InProgress)
	{
		BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
		if(BlasterHUD == nullptr)	return;

		BlasterHUD->AddCharacterOverlay();
	}
}