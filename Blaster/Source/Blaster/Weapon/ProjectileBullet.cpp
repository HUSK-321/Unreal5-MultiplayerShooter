// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBullet.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpluse, const FHitResult& Hit)
{
	auto OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(OwnerCharacter == nullptr)	return;
	
	AController* OwnerController = OwnerCharacter->GetController();
	if(OwnerController == nullptr)	return;
	
	UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpluse, Hit);
}