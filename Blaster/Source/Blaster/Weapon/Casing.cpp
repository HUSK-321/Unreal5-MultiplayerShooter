// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"

ACasing::ACasing()
	:
	CasingMesh(CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CashingMesh")))
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CasingMesh);
	
}

void ACasing::BeginPlay()
{
	Super::BeginPlay();
	
}


