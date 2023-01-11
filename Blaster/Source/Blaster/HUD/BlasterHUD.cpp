// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"

#include "CharacterOverlay.h"
#include "Blueprint/UserWidget.h"

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();
}

void ABlasterHUD::AddCharacterOverlay()
{
	auto PlayerController = GetOwningPlayerController();
	if(PlayerController == nullptr) return;

	if(CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
	
}

void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if(!GEngine)	return;
	
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter{ ViewportSize.X / 2.f, ViewportSize.Y / 2.f };

	FVector2D Spread;
	float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;
	if(HUDPackage.CrosshairsCenter)
	{
		Spread.X = 0.f;
		Spread.Y = 0.f;
		DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairColor);
	}
	if(HUDPackage.CrosshairsLeft)
	{
		Spread.X = -SpreadScaled;
		Spread.Y = 0.f;
		DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairColor);
	}
	if(HUDPackage.CrosshairsRight)
	{
		Spread.X = SpreadScaled;
		Spread.Y = 0.f;
		DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairColor);
	}
	if(HUDPackage.CrosshairsBottom)
	{
		Spread.X = 0.f;
		Spread.Y = SpreadScaled;
		DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairColor);
	}
	if(HUDPackage.CrosshairsTop)
	{
		Spread.X = 0.f;
		Spread.Y = -SpreadScaled;
		DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairColor);
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2d TextureDrawPoint{ ViewportCenter.X - (TextureWidth / 2.f) + Spread.X, ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y };

	DrawTexture(Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight,
		0.f, 0.f, 1.f, 1.f,
		CrosshairColor);
}
