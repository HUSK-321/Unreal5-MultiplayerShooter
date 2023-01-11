// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Blaster/Blaster.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

AProjectile::AProjectile()
	:
	CollisionBox(CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"))),
	ProjectileMovementComponent(CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent")),
	Damage(20.f)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SetRootComponent(CollisionBox);

	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);
	
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}

	if(Tracer == nullptr)	return;
	TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBox, FName(),
										GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpluse, const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

