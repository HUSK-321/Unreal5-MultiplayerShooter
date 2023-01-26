#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssultRifle UMETA(Displayname = "Assult Rifle"),
	EWT_RocketLauncher UMETA(Displayname = "Rocket Launcher"),
	EWT_Pistol UMETA(Displayname = "Pistol"),

	EWT_MAX UMETA(Displayname = "DefaultMAX")
};
