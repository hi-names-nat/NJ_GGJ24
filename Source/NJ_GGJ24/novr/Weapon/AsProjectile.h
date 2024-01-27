// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AsProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class NJ_GGJ24_API AAsProjectile : public AActor
{
	GENERATED_BODY()
public:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon, meta = (AllowPrivateAccess = "true", UIMin="1.0", UIMax="100.0"))
	float ProjectileDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Visual, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Visual, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MissParticle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Visual, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* MissDecal;

public:
	AAsProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	
};

