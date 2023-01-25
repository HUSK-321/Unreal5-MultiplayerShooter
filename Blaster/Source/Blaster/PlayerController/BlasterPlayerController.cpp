// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"

#include "Blaster/BlasterComponents/CombatComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/HUD/Announcement.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Kismet/GameplayStatics.h"


void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BlasterHUD = Cast<ABlasterHUD>(GetHUD());
	
	ServerCheckMatchState();
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

bool ABlasterPlayerController::CharacterOverlayHUDIsValid()
{
	return (BlasterHUD && BlasterHUD->CharacterOverlay);
}

void ABlasterPlayerController::SetHUDTime()
{
	float TimeLeft = 0.f;
	if(MatchState == MatchState::WaitingToStart)	TimeLeft = WarmupTime - GetServerTime() + LevelStartingTime;
	else if(MatchState == MatchState::InProgress)   TimeLeft = WarmupTime + MatchTime - GetServerTime() + LevelStartingTime;
	else if(MatchState == MatchState::Cooldown)		TimeLeft = CooldownTime + MatchTime - GetServerTime() + LevelStartingTime;
	
	uint32 SecondsLeft = FMath::CeilToInt(TimeLeft);

	if(HasAuthority())
	{
		BlasterGameMode = BlasterGameMode == nullptr ? Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this)) : BlasterGameMode;
		if(BlasterGameMode)
		{
			SecondsLeft = BlasterGameMode->GetCountdownTime();
		}
	}
	
	if (CountdownInt != SecondsLeft)
	{
		if(MatchState == MatchState::WaitingToStart || MatchState == MatchState::Cooldown)
		{
			SetHUDAnnouncementCountdown(TimeLeft);
		}
		else if(MatchState == MatchState::InProgress)
		{
			SetHUDMatchCountdown(TimeLeft);
		}
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
	if(!CharacterOverlayHUDIsValid() || BlasterHUD->CharacterOverlay->HealthBar == nullptr || BlasterHUD->CharacterOverlay->HealthText == nullptr)
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
	if(!CharacterOverlayHUDIsValid() || BlasterHUD->CharacterOverlay->ScoreAmount == nullptr)
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
	if(!CharacterOverlayHUDIsValid() || BlasterHUD->CharacterOverlay->DefeatsAmount == nullptr)
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
	if(!CharacterOverlayHUDIsValid() || BlasterHUD->CharacterOverlay->WeaponAmmoAmount == nullptr)	return;

	FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
	BlasterHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
}

void ABlasterPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!CharacterOverlayHUDIsValid() || BlasterHUD->CharacterOverlay->CarriedAmmoAmount == nullptr)	return;

	FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
	BlasterHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
}

void ABlasterPlayerController::SetHUDMatchCountdown(float CountdownTime)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(!CharacterOverlayHUDIsValid() || BlasterHUD->CharacterOverlay->MatchCountdownText == nullptr)	return;

	if(CountdownTime < 0.f)
	{
		BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText());
		return;
	}

	int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
	int32 Seconds = FMath::FloorToInt(CountdownTime - Minutes * 60.f);
	FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
	BlasterHUD->CharacterOverlay->MatchCountdownText->SetText(FText::FromString(CountdownText));
}

void ABlasterPlayerController::SetHUDAnnouncementCountdown(float CountdownTime)
{
	BlasterHUD = (BlasterHUD == nullptr) ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	bool bHUDValid = BlasterHUD && BlasterHUD->Announcement && BlasterHUD->Announcement->WarmupTime;
	if(!bHUDValid)	return;

	if(CountdownTime < 0.f)
	{
		BlasterHUD->Announcement->WarmupTime->SetText(FText());
		return;
	}

	int32 Minutes = FMath::FloorToInt(CountdownTime / 60.f);
	int32 Seconds = FMath::FloorToInt(CountdownTime - Minutes * 60.f);
	FString CountdownText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
	BlasterHUD->Announcement->WarmupTime->SetText(FText::FromString(CountdownText));
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
		HandleMatchHasStarted();
	}
	else if(MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void ABlasterPlayerController::OnRep_MatchState()
{
	if(MatchState == MatchState::InProgress)
	{
		HandleMatchHasStarted();
	}
	else if(MatchState == MatchState::Cooldown)
	{
		HandleCooldown();
	}
}

void ABlasterPlayerController::HandleMatchHasStarted()
{
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(BlasterHUD == nullptr)	return;

	BlasterHUD->AddCharacterOverlay();
	if(BlasterHUD->Announcement)
	{
		BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ABlasterPlayerController::HandleCooldown()
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(GetPawn());
	if(BlasterCharacter)
	{
		BlasterCharacter->bDisableGamePlay = true;
		BlasterCharacter->GetCombat()->FireButtonPressed(false);
	}
	
	BlasterHUD = BlasterHUD == nullptr ? Cast<ABlasterHUD>(GetHUD()) : BlasterHUD;
	if(BlasterHUD == nullptr)	return;

	BlasterHUD->CharacterOverlay->RemoveFromParent();

	bool bAnnouncementHUDValid = BlasterHUD->Announcement && BlasterHUD->Announcement->AnnouncementText && BlasterHUD->Announcement->InfoText;
	if(!bAnnouncementHUDValid)	return;
	
	BlasterHUD->Announcement->SetVisibility(ESlateVisibility::Visible);
	FString AnnouncementText("New Match Starts In : ");
	BlasterHUD->Announcement->AnnouncementText->SetText(FText::FromString(AnnouncementText));

	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* BlasterPlayerState = GetPlayerState<ABlasterPlayerState>();
	if(BlasterGameState && BlasterPlayerState)
	{
		FString InfoTextString;
		TArray<ABlasterPlayerState*> TopPlayers = BlasterGameState->TopScoringPlayers;
		if(TopPlayers.Num() == 0)
		{
			InfoTextString = FString("There is no winner T T");
		}
		else if(TopPlayers.Num() == 1 && TopPlayers[0] == BlasterPlayerState)
		{
			InfoTextString = FString("You are the winner");
		}
		else if(TopPlayers.Num() == 1)
		{
			InfoTextString = FString::Printf(TEXT("Winner : \n%s"), *TopPlayers[0]->GetPlayerName());
		}
		else if(TopPlayers.Num() > 1)
		{
			InfoTextString = FString("Players tied for the win : \n");
			for(auto TidePlayer : TopPlayers)
			{
				InfoTextString.Append(FString::Printf(TEXT("%s\n"), *TidePlayer->GetPlayerName()));
			}
		}
	BlasterHUD->Announcement->InfoText->SetText(FText::FromString(InfoTextString));
	}
}

void ABlasterPlayerController::ServerCheckMatchState_Implementation()
{
	ABlasterGameMode* GameMode = Cast<ABlasterGameMode>(UGameplayStatics::GetGameMode(this));
	if(GameMode == nullptr)	return;

	WarmupTime = GameMode->WarmupTime;
	MatchTime = GameMode->MatchTime;
	CooldownTime = GameMode->CooldownTime;
	LevelStartingTime = GameMode->LevelStartingTime;
	MatchState = GameMode->GetMatchState();

	ClientJoinMidGame(MatchState, WarmupTime, MatchTime, CooldownTime, LevelStartingTime);
}

void ABlasterPlayerController::ClientJoinMidGame_Implementation(FName StateOfMatch, float Warmup, float Match, float cooldown, float StartingTime)
{
	WarmupTime = Warmup;
	MatchTime = Match;
	LevelStartingTime = StartingTime;
	CooldownTime = cooldown;
	MatchState = StateOfMatch;
	OnMatchStateSet(MatchState);

	if(BlasterHUD && MatchState == MatchState::WaitingToStart)
	{
		BlasterHUD->AddAnnouncement();
	}
}