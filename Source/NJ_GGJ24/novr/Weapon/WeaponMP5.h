// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "WeaponMP5.generated.h"

UCLASS()
class NJ_GGJ24_API AWeaponMP5 : public AWeaponActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponMP5();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool Fire() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	float BulletDamage;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
