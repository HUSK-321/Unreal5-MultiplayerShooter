// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"

#include "Kismet/GameplayStatics.h"

AProjectileRocket::AProjectileRocket()
	:
	RocketMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh")))
{
	RocketMesh->SetupAttachment(GetRootComponent());
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpluse, const FHitResult& Hit)
{
	APawn* FiringPawn = GetInstigator();
	if(FiringPawn)
	{
		AController* FiringController = FiringPawn->GetController();
		if(FiringPawn)
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 10.f, GetActorLocation(),
												200.f, 500.f, 1.f, UDamageType::StaticClass(), TArray<AActor*>(), this,
												FiringController);
			
		}
	}
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpluse, Hit);
}
