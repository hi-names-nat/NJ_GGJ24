// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBackpack.h"

#define UE_UINT_MAX 255

// Sets default values for this component's properties
UAmmoBackpack::UAmmoBackpack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAmmoBackpack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAmmoBackpack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UAmmoBackpack::GetReserve(EAmmoType AmmoType)
{return UE_UINT_MAX;}

int UAmmoBackpack::GetAmmoOut(EAmmoType AmmoType, int Amount)
{return Amount;}

