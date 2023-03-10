#pragma once

#define TRACE_LENGTH 80000.f
#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssultRifle UMETA(Displayname = "Assult Rifle"),
	EWT_RocketLauncher UMETA(Displayname = "Rocket Launcher"),
	EWT_Pistol UMETA(Displayname = "Pistol"),
	EWT_SubmachineGun UMETA(Displayname = "SubmachineGun"),
	EWT_Shotgun UMETA(Displayname = "Shotgun"),
	EWT_SniperRifle UMETA(Displayname = "SniperRifle"),
	EWT_GrenadeLAuncher UMETA(Displayname = "GrenadeLAuncher"),

	EWT_MAX UMETA(Displayname = "DefaultMAX")
};
