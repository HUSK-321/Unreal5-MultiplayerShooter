// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "WeaponTypes.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr)	return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if(MuzzleFlashSocket == nullptr)	return;
	
	FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	FVector Start = SocketTransform.GetLocation();
	FVector End = Start + (HitTarget - Start) * 1.25f;

	FHitResult FireHit;
	UWorld* World = GetWorld();
	if(World == nullptr)	return;
	
	World->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);

	FVector BeamEnd = End;
	if(FireHit.bBlockingHit)
	{
		BeamEnd = FireHit.ImpactPoint;
		ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
		if(HasAuthority() && BlasterCharacter && InstigatorController)
		{
			UGameplayStatics::ApplyDamage(BlasterCharacter, Damage, InstigatorController, this, UDamageType::StaticClass());
		}
		if(ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
		}
		if(HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(World, HitSound, FireHit.ImpactPoint);
		}
	}
	if(BeamParticles)
	{
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, SocketTransform);
		if(Beam == nullptr)	return;

		Beam->SetVectorParameter(FName("Target"), BeamEnd);
	}
	if(MuzzleFlash)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, MuzzleFlash, SocketTransform);
	}
	if(FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, FireSound, GetActorLocation());
	}
}

FVector AHitScanWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget)
{
	FVector TotargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	FVector SphereCenter = TraceStart + TotargetNormalized * DistanceToSphere;
	FVector RandVector = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	FVector EndLocation = SphereCenter + RandVector;
	FVector ToEndLocation = EndLocation - TraceStart;

	return FVector(TraceStart + ToEndLocation * TRACE_LENGTH / ToEndLocation.Size());
}
