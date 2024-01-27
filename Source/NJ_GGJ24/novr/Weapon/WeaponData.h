// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "UObject/Object.h"
#include "WeaponData.generated.h"

/**
 * Enum of weapon types. Not the best way to do things but
 */
UENUM(BlueprintType)
enum struct EWeaponType: uint8
{
	Pistol	= 0,
	Smg		= 1,
	Shotgun = 2,
	Carbine = 3,
	Rifle	= 4,
	Sniper	= 5,
};


/**
 * 
 */
UCLASS()
class UWeaponData : public UObject
{
	GENERATED_BODY()

public:
	UWeaponData();
	UPROPERTY(EditDefaultsOnly)
	FString Name;
	UPROPERTY(EditDefaultsOnly)
	FString Description;
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType;
	// UPROPERTY(EditDefaultsOnly)
	// UImage* UIImage;
};
