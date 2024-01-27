// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NJ_GGJ24/System/Character/HealthComponent.h"

AAsProjectile::AAsProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AAsProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

}

void AAsProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics


	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UParticleSystem* Particle = MissParticle;

		if (auto health = Cast<UHealthComponent>(OtherComp))
		{
			health->TakeDamage(ProjectileDamage);
			Particle = HitParticle;
		}
		else 
		{
			if (OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			}
			if (MissDecal)
			{
				UGameplayStatics::SpawnDecalAttached(
				MissDecal,
				FVector(20),
				Cast<USceneComponent>(OtherComp),
				TEXT("NAME_None"), GetActorLocation(),
				NormalImpulse.Rotation() + UE::Math::TRotator<double>(0,0,FMath::RandRange(0, 359)),
				EAttachLocation::KeepWorldPosition
				);
			}
		}
		if (Particle)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit %s"), *OtherActor->GetName())
			UGameplayStatics::SpawnEmitterAtLocation(
				this,
				Particle,
				Hit.Location,
				FRotator::ZeroRotator,
				FVector(1));
		}
	}
}