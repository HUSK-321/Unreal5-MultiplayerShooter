// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairs.h"
#include "Components/TimelineComponent.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairs
{
	GENERATED_BODY()

public:
	
	ABlasterCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void OnRep_ReplicatedMovement() override;

	virtual void Destroyed() override;

	void Elim();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

protected:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void UpdateHUDHealth();

	virtual void BeginPlay() override;

	void Moveforward(float Value);

	void MoveRight(float Value);

	void Turn(float Value);

	void LookUp(float Value);

	void EquipButtonPressed();
	
	void CrouchButtonPressed();

	void ReloadButtonPressed();

	void AimButtonPressed();

	void AimButtonReleased();

	void FireButtonPressed();

	void FireButtonReleased();
	
	void CalculateAO_Pitch();
	
	float ClaculateSpeed();

	void AimOffset(float DeltaTime);

	void SimProxiesTurn();

	virtual void Jump() override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	/** Pool for any relevant classes and initialize hud */
	void PollInit();

private:

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverheadWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<class AWeapon> OverlappingWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCombatComponent> Combat;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	float AO_Yaw;
	float InterpAO_Yaw;

	float AO_Pitch;

	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<class UAnimMontage> FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TObjectPtr<UAnimMontage> ElimMontage;

	void PlayHitReactMontage();
	
	void HideCameraWhenCharacterIsClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold;

	bool bRotateRootBone;

	float TurnThreshold;

	FRotator ProxyRotationLastFrame;

	FRotator ProxyRotation;

	float ProxyYaw;

	float TimeSinceLastMovementReplication;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health;
	
	bool bElimmed;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay;

	void ElimTimerFinished();

	/** Dissolve Effect */

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimeline;

	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> DissolveCurve;

	// Dynamic instance we cna change in runtime
	UPROPERTY(VisibleAnywhere, Category = Elim)
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;

	// Material instance set o the blueprint, used with the dynamic material instance
	UPROPERTY(EditAnywhere, Category = Elim)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, Category = Elim)
	TObjectPtr<UParticleSystem> ElimBotEffect;

	UPROPERTY(VisibleAnywhere, Category = Elim)
	TObjectPtr<UParticleSystemComponent> ElimBotComponent;

	UPROPERTY(EditAnywhere, Category = Elim)
	TObjectPtr<class USoundCue> ElimBotSound; 
	
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	void StartDissolve();
	
	UFUNCTION()
	void OnRep_Health();

	UPROPERTY()
	TObjectPtr<class ABlasterPlayerController> BlasterPlayerController;

	UPROPERTY()
	TObjectPtr<class ABlasterPlayerState> BlasterPlayerState;
	
public:	

	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();

	bool IsAiming();

	void PlayFireMontage(bool bAiming);

	void PlayElimMontage();

	void PlayReloadMontage();

	FORCEINLINE float GetAOYaw() { return AO_Yaw; }
	FORCEINLINE float GetAOPitch() { return AO_Pitch; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return  FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return  bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return  bElimmed; }
	AWeapon* GetEquippedWeapon();

	FVector GetHitTarget() const;
	
	ECombatState GetCombatState() const;
};
