// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMP5.h"

#include "Camera/CameraComponent.h"
#include "NJ_GGJ24/novr/Player/AsPlayer.h"
#include "NJ_GGJ24/System/Character/HealthComponent.h"


// Sets default values
AWeaponMP5::AWeaponMP5()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponMP5::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AWeaponMP5::Fire()
{
	if (!Super::Fire()) return false;

	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
	FHitResult Out_Hit;
		FVector TargetLocation = SpawnLocation + (Player->GetCamera()->GetForwardVector() * MaxRange);

		auto world = GetWorld();
		//Trace using the hitscan channel
		if (world->LineTraceSingleByChannel(Out_Hit, SpawnLocation, TargetLocation, ECC_GameTraceChannel2))
		{
			DrawDebugSphere(world, Out_Hit.Location, 15, 8, FColor::Red, false, 10);
			DrawDebugLine(world, SpawnLocation, TargetLocation, FColor::Red, false, 10);
			// UParticleSystem* Particle = MissParticle;
			if (auto healthComponent = Cast<UHealthComponent>(Out_Hit.Component))
			{
				healthComponent->TakeDamage(BulletDamage);
				// Particle = BloodParticle;
			}
			else
			{
				if (Out_Hit.Component->IsSimulatingPhysics())
				{
					auto dir = TargetLocation-SpawnLocation;
					Out_Hit.Component->AddImpulseAtLocation(dir * 25.F, Out_Hit.Location);
				}
				// if (BulletHole)
				// {
				// 	UGameplayStatics::SpawnDecalAttached(
				// 	BulletHole,
				// 	FVector(20),
				// 	Cast<USceneComponent>(Out_Hit.Component),
				//     TEXT("NAME_None"), Out_Hit.Location,
				// 	Out_Hit.Normal.Rotation() + UE::Math::TRotator<double>(0,0,FMath::RandRange(0, 359)),
				// 	EAttachLocation::KeepWorldPosition
				// 	);
				// }
			}	
			// if (Particle)
			// {
			// 	UGameplayStatics::SpawnEmitterAttached(
			// 	 Particle,
			// 	 Cast<USceneComponent>(Out_Hit.Component),
			// 	 TEXT("NAME_None"),
			// 	 Out_Hit.Location,
			// 	 FRotator::ZeroRotator,
			// 	 FVector(1),
			// 	 EAttachLocation::KeepWorldPosition
			// 	 );
			// }
		}	
	return true;
}

// Called every frame
void AWeaponMP5::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

