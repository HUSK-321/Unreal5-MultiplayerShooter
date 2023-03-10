// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"


USTRUCT()
struct FHUDPackage
{
	GENERATED_BODY()
	
public:
	TObjectPtr<class UTexture2D> CrosshairsCenter;
	TObjectPtr<class UTexture2D> CrosshairsLeft;
	TObjectPtr<class UTexture2D> CrosshairsRight;
	TObjectPtr<class UTexture2D> CrosshairsTop;
	TObjectPtr<class UTexture2D> CrosshairsBottom;

	float CrosshairSpread;

	FLinearColor CrosshairColor;
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category="Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	UPROPERTY(EditAnywhere, Category="Player Stats")
	TSubclassOf<UUserWidget> AnnouncementClass;

	UPROPERTY()
	TObjectPtr<class UCharacterOverlay> CharacterOverlay;

	UPROPERTY()
	TObjectPtr<class UAnnouncement> Announcement;

protected:

	virtual void BeginPlay() override;

private:

	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax{ 16.f };

public:

	FORCEINLINE void SetHUDPackage (const FHUDPackage& Package) { HUDPackage = Package; }
	
	void AddCharacterOverlay();

	void AddAnnouncement();
};
