// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	AProjectile();
	
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent>	CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> Tracer;

	TObjectPtr<class UParticleSystemComponent> TracerComponent;
};
