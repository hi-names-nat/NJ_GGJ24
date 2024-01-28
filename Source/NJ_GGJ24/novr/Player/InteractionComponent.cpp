// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "AsPlayer.h"
#include "Camera/CameraComponent.h"
#include "NJ_GGJ24/System/InteractInterface.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UInteractionComponent::AttemptInteract()
{
	auto Owner = Cast<AAsPlayer>(GetOwner());
	auto CameraPos = Owner->GetCamera()->GetComponentLocation();
	auto CameraForward = Owner->GetCamera()->GetForwardVector();
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FHitResult Out_Hit;

	auto World = GetWorld();
	
	if (!World->LineTraceSingleByChannel(Out_Hit, CameraPos, CameraPos +
		(CameraForward * InteractReach), ECC_GameTraceChannel1, CollisionParams)) return;
	DrawDebugLine(World, CameraPos, CameraPos + (CameraForward * InteractReach), FColor::Red,
		false, 5);
	UE_LOG(LogTemp, Log, TEXT("I touched %s"), *Out_Hit.GetComponent()->GetName());
	if (auto t = Cast<IInteractInterface>(Out_Hit.Component))
	{
		t->Interact(Cast<APlayerController>(Owner->GetController()));
	}
	else if (auto c = Cast<IInteractInterface>(Out_Hit.Component->GetOwner()))
	{
		c->Interact(Cast<APlayerController>(Owner->GetController()));
	}
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}