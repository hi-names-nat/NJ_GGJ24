// (c) Dark Frog SGS 2023.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.generated.h"

class USoundCue;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKilled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHurt);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NJ_GGJ24_API UHealthComponent : public UBoxComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = Callbacks)
	FOnKilled OnKilled;
	
	UPROPERTY(BlueprintAssignable, Category = Callbacks)
	FOnKilled OnHurt;
	
	UPROPERTY(EditDefaultsOnly, Category=Health)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category=Health)
	float CurrentHealth;
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Amount);

	UFUNCTION(BlueprintCallable)
	void HealDamage(float Amount) {CurrentHealth += Amount; if (CurrentHealth > MaxHealth) CurrentHealth = MaxHealth;}

	void SetHealth(float Amount) {CurrentHealth = Amount;}
	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
};
