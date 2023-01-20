// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial		UMETA(DisplayName = "Initial State"),
	EWS_Equipped	UMETA(DisplayName = "EQuipped"),
	EWS_Dropped		UMETA(DisplayName = "Dropped"),

	EWS_MAX			UMETA(DisplayName = "DefaultMAX")
	
};

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnRep_Owner() override;
	
	void SetHUDAmmo();
	
	/** Textures for weapon */
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<class UTexture2D> CrosshairsCenter;
	
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsLeft;
	
	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
	TObjectPtr<UTexture2D> CrosshairsBottom;

	/** Zoom FOV while aiming */
	UPROPERTY(EditAnywhere)
	float ZoomFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic;

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<class USphereComponent> AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UWidgetComponent> PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UAnimationAsset> FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_Ammo)
	int32 Ammo;

	UFUNCTION()
	void OnRep_Ammo();

	void SpendRound();
	
	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	UPROPERTY()
	TObjectPtr<class ABlasterCharacter> BlasterOwnerCharacter;

	UPROPERTY()
	TObjectPtr<class ABlasterPlayerController> BlasterOwnerController;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
public:
	
	void ShowPickupWidget(bool bShowWidget);

    UFUNCTION()
	void OnRep_WeaponState();

	void SetWeaponState(EWeaponState State);

	virtual void Fire(const FVector& HitTarget);

	void AddAmmo(int32 AmmoToAdd);

	void Drop();

	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	FORCEINLINE float GetZoomedFOV() const { return ZoomFOV; }

	FORCEINLINE float GetZoomInterpSpeed() const { return ZoomInterpSpeed; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }

	FORCEINLINE int32 GetAmmo() { return Ammo; }

	FORCEINLINE int32 GetMagCapacity() { return MagCapacity; }

	bool IsEmpty();
};
