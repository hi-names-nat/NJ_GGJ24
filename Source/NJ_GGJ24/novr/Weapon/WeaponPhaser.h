#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"

UCLASS()
class NJ_GGJ24_API AWeaponPhaser : public AWeaponActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponPhaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool Fire() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	float RayDamage;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
