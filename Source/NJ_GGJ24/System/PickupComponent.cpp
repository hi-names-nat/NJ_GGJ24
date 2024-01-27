// Copyright Epic Games, Inc. All Rights Reserved.

#include "PickupComponent.h"

#include "NJ_GGJ24/novr/Player/AsPlayer.h"

UPickupComponent::UPickupComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UPickupComponent::DisableComponent()
{
	// Unregister from the Overlap Event so it is no longer triggered
	OnComponentBeginOverlap.RemoveAll(this);
}

void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UPickupComponent::OnSphereBeginOverlap);
}

void UPickupComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	if(const auto Character = Cast<AAsPlayer>(OtherActor))
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Cast<APlayerController>(Character->GetController()));
	}
}
