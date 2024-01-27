// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponPhaser.h"

#include "Camera/CameraComponent.h"
#include "NJ_GGJ24/novr/Player/AsPlayer.h"
#include "NJ_GGJ24/System/Character/HealthComponent.h"


// Sets default values
AWeaponPhaser::AWeaponPhaser()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponPhaser::BeginPlay()
{
	Super::BeginPlay();
}

bool AWeaponPhaser::Fire()
{
	if (!Super::Fire()) return false;

	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
	FHitResult Out_Hit;
	FVector TargetLocation = SpawnLocation + (Player->GetCamera()->GetForwardVector() * MaxRange);

	auto world = GetWorld();
	// Trace using the hitscan channel
	if (world->LineTraceSingleByChannel(Out_Hit, SpawnLocation, TargetLocation, ECC_GameTraceChannel2))
	{
		DrawDebugSphere(world, Out_Hit.Location, 15, 8, FColor::Red, false, 10);
		DrawDebugLine(world, SpawnLocation, TargetLocation, FColor::Red, false, 10);
		if (auto healthComponent = Cast<UHealthComponent>(Out_Hit.Component))
		{
			healthComponent->TakeDamage(BulletDamage);
		}
		else
		{
			if (Out_Hit.Component->IsSimulatingPhysics())
			{
				auto dir = TargetLocation-SpawnLocation;
				Out_Hit.Component->AddImpulseAtLocation(dir * 25.F, Out_Hit.Location);
			}
	}
	return true;
}

// Called every frame
void AWeaponPhaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

