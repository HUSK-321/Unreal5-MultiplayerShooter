// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blaster//HUD/BlasterHUD.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UCombatComponent();
	friend class ABlasterCharacter;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();
	
	void Fire();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	
	TObjectPtr<ABlasterCharacter> Character;

	TObjectPtr<class ABlasterPlayerController> Controller;

	TObjectPtr<class ABlasterHUD> HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TObjectPtr<class AWeapon> EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/** Hud and Crosshairs */
	
	FHUDPackage HUDPackage;
	
	float CrosshairVelocityFactor;

	float CrosshairInAirFactor;

	float CrosshairAimFactor;

	float CrosshairShootingFactor;

	FVector HitTarget;

	/** Aiming and FOV */
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomFOV{ 30.f };

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed{ 20.f };

	float CurrentFOV;

	void InterpFOV(float DeltaTime);

	/** For automatic fire */

	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bCanFire;

	void StartFireTimer();

	void FireTimerFinished();
	
public:
	
	void EquipWeapon(AWeapon* WeaponToEquip);
		
};
