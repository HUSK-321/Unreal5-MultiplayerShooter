// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

AWeapon::AWeapon()
	:
	WeaponMesh(CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"))),
	AreaSphere(CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"))),
	PickupWidget(CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget")))
	
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere->SetupAttachment(GetRootComponent());
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
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

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		break;

	case EWeaponState::EWS_Dropped:

		break;
	}
}



