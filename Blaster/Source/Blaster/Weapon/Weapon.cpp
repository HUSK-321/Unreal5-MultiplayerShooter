// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AWeapon::AWeapon()
	:
	WeaponMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"))),
	AreaShpere(CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"))),
	PickupWidget(CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget")))
	
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaShpere->SetupAttachment(GetRootComponent());
	AreaShpere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaShpere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	if(HasAuthority())
	{
		AreaShpere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaShpere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		AreaShpere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		AreaShpere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
	
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto BlasterCharacter{ Cast<ABlasterCharacter>(OtherActor) };
	if(BlasterCharacter == nullptr)	return;

	BlasterCharacter->SetOverlappingWeapon(this);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto BlasterCharacter{ Cast<ABlasterCharacter>(OtherActor) };
	if(BlasterCharacter == nullptr)	return;

	BlasterCharacter->SetOverlappingWeapon(nullptr);
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if(PickupWidget == nullptr)	return;

	PickupWidget->SetVisibility(bShowWidget);
}



