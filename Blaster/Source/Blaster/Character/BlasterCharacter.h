// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairs.h"
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

	UFUNCTION(NetMulticast, Reliable)
	void Elim();

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

private:

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverheadWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<class AWeapon> OverlappingWeapon;

	UPROPERTY(VisibleAnywhere)
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

	UFUNCTION()
	void OnRep_Health();

	TObjectPtr<class ABlasterPlayerController> BlasterPlayerController;
	
public:	

	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();

	bool IsAiming();

	void PlayFireMontage(bool bAiming);

	void PlayElimMontage();

	FORCEINLINE float GetAOYaw() { return AO_Yaw; }
	FORCEINLINE float GetAOPitch() { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return  FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return  bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return  bElimmed; }
	AWeapon* GetEquippedWeapon();

	FVector GetHitTarget() const;
};
