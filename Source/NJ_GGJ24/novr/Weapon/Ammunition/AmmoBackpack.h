// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoBackpack.generated.h"

UENUM()
enum struct EAmmoType : uint8
{
	_9x19para = 0,
	_45acp = 1,
	_556nato = 2,
	buckshot = 3,
	_762nato = 4,
	phaser = 5
};

UENUM()
enum EAmmoModifier
{
	fmj,
	hp,
	energy
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NJ_GGJ24_API UAmmoBackpack : public UActorComponent
{
	GENERATED_BODY()


public:
	// Sets default values for this component's properties
	UAmmoBackpack();

	UPROPERTY(EditAnywhere)
	bool bInfAmmo = false;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// Todo
	int GetReserve(EAmmoType AmmoType);
	int GetAmmoOut(EAmmoType AmmoType, int Amount);
};
