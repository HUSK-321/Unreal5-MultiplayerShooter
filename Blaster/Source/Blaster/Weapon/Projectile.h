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

	virtual void Destroyed() override;

protected:
	
	virtual void BeginPlay() override;

	void StartDestroyTimer();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpluse, const FHitResult& Hit);
	
	void DestroyTimerFinished();

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> Tracer;

	UPROPERTY()
	TObjectPtr<class UParticleSystemComponent> TracerComponent;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactParticles;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent>	CollisionBox;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> TrailSystem;

	UPROPERTY()
	TObjectPtr<class UNiagaraComponent> TrailSystemComponent;
	
	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(EditAnywhere)
	float DamageInnerRadius;
	UPROPERTY(EditAnywhere)
	float DamageOuterRadius;

	void SpawnTrailSystem();

	void ExplodeDamage();
};
