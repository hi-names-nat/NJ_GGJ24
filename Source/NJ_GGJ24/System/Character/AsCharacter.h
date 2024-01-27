// (c) Dark Frog SGS 2023.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "GameFramework/Character.h"
#include "AsCharacter.generated.h"

class USoundCue;

UCLASS()
class NJ_GGJ24_API AAsCharacter : public ACharacter
{
public:

private:
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAsCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	bool PlayAudioFromComponent(TObjectPtr<USoundBase> SoundCue);

	UPROPERTY(EditDefaultsOnly, Category=Sound)
	TObjectPtr<USoundBase> DamageSound;
	UPROPERTY(EditDefaultsOnly, Category=Sound)
	TObjectPtr<USoundBase> DieSound;

	UFUNCTION(BlueprintCallable)
	int GetHealth() const {return HealthComponent->CurrentHealth;}
	UFUNCTION(BlueprintCallable)
	int GetMaxHealth() const {return HealthComponent->MaxHealth;}
	UFUNCTION(BlueprintCallable)
	void SetHealth(int Amount);
	UFUNCTION(BlueprintCallable)
	void RecoverHealth(int Amount);
	UFUNCTION(BlueprintCallable)
	void DealHealth(int Amount);
	UFUNCTION(BlueprintNativeEvent)
	void OnDie();
	UFUNCTION(BlueprintNativeEvent)
	void OnHurt();

	
};
