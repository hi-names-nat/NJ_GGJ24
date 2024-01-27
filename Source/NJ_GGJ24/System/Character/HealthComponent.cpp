// (c) Dark Frog SGS 2023.


#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"


void UHealthComponent::TakeDamage(float Amount)
{
	CurrentHealth -= Amount;
	UE_LOG(LogTemp, Log, TEXT("Took Damage. New Health: %d"), CurrentHealth);
	if (CurrentHealth <= 0)
	{
		OnKilled.Broadcast();
	}
	else
	{
		OnHurt.Broadcast();
	}
}

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}