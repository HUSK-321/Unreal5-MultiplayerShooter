// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon()
	:
	WeaponMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"))),
	AreaShpere(CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere")))
	
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh->SetupAttachment(GetRootComponent());
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaShpere->SetupAttachment(GetRootComponent());
	AreaShpere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaShpere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		AreaShpere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaShpere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
	
}

