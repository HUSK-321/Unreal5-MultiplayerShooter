// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

private:
	
	TObjectPtr<ABlasterCharacter> Character;

	UPROPERTY(Replicated)
	TObjectPtr<class AWeapon> EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;
	
public:
	
	void EquipWeapon(AWeapon* WeaponToEquip);
		
};
