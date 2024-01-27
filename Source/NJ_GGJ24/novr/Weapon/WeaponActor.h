// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NJ_GGJ24/System/InteractInterface.h"
#include "WeaponActor.generated.h"

class UPickupComponent;
struct FEnhancedInputActionEventBinding;
class UInteractionComponent;
enum struct EAmmoType : uint8;
class UInputAction;
class UInputMappingContext;
class UWeaponData;

UCLASS()
class NJ_GGJ24_API AWeaponActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	////////////////////////////////////
/// Member Variables 
////////////////////////////////////
public: 
protected:
	UPROPERTY()
	class AAsPlayer* Player;
	
	////////////////////
	///////	Input
	////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Primary Fire", Category="Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* FireAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Secondary Fire", Category="Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* SecondaryAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Reload", Category="Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/**
	 * @brief Action binding for begin & end fire. Used for animation.
	 */
	FEnhancedInputActionEventBinding* BeginFireBinding;
	FEnhancedInputActionEventBinding* EndFireBinding;
	FEnhancedInputActionEventBinding* ReloadBinding;


	////////////////////
	///////	Weapon Properties
	////////////////////
	UPROPERTY(EditAnywhere, Category="Ammo")
	int MagCapacity=0;
	UPROPERTY(EditAnywhere, Category="Ammo")
	int InitialAmmo=0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Ammo")
	int CurrentMagAmmo=0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ammo")
	float MaxRange;	
	
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	float TimeBetweenShots = .4;
	UPROPERTY(EditAnywhere, Category="Properties")
	bool IsFullAuto = false;
	/**
	* @brief Offset for where the weapon fires from.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	FVector MuzzleOffset;
	
	UPROPERTY(VisibleAnywhere, Category="Properties")
	bool bCanFire = true;
	/**
	 * @brief if the player is holding the fire key/button.
	 */
	UPROPERTY(VisibleAnywhere, Category="Innards")
	bool FireHeld = false;
	/**
	 * @brief If the player has the InfiniteAmmo debug setting on.
	 */
	UPROPERTY(VisibleAnywhere, Category="Innards")
	bool InfiniteAmmo = false;

	/**
	 * @brief handler for the fire cooldown.
	 */
	FTimerHandle TimerHandle;

	/**
	 * @brief Delegate for fire cooldown.
	 */
	FTimerDelegate TimerDelegate;
	/**
	 * @brief Delegate for after the fire cooldown expires.
	 */
	FTimerDelegate EndDelegate;
	
	////////////////////
	///////	Visual
	////////////////////
public:

	/**
	* @brief The first-person weapon viewmodel
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Visual")
	USkeletalMeshComponent* WeaponViewmodel;
	/**
	* @brief The model for the weapon when not equipped
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual")
	UStaticMeshComponent* WeaponWorldmodel;

	////////////////////
	///////	World
	////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual")
	UPickupComponent* Interaction;
protected:
	/**
	* @brief The data that the UI uses for the weapon's name, description,
	* type, and sprite.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Visual")
	UWeaponData* WeaponData;

	////////////////////
	/////// Animation
	////////////////////
	/**
	* @brief Blend space that contains the idle/walk/run/sprint/reload/fire viewmodel animations
	*/
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UBlendSpace* ViewmodelBlendSpace;
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimationAsset* ViewmodelEquipAnim;
	
	////////////////////
	///////	Sound
	////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* NoAmmoSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundBase* ReloadSound;

	////////////////////
	///////	Equip
	////////////////////
	bool isOwned = false;

private:
	
	////////////////////////////////////
	/// Member Methods 
	////////////////////////////////////

	////////////////////
	/////// Setup/Teardown
	////////////////////
public:
	AWeaponActor();


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	////////////////////
	/////// Firing
	////////////////////
	UFUNCTION(BlueprintCallable, Category="Firing")
	virtual void BeginFire();
	UFUNCTION(BlueprintCallable, Category="Firing")
	virtual bool Fire();
	UFUNCTION(BlueprintCallable, Category="Firing")
	virtual void EndFire();
	/**
	* @brief Repeats the fire command for automatic weapons
	*/
	void FireLoop();

	void FORCEINLINE ResetFireCooldown()
	{
		bCanFire = true;
	}

	////////////////////
	/////// Reload/Equip
	////////////////////
	UFUNCTION(BlueprintNativeEvent, Category="Reload")
	void Reload();

	UFUNCTION(BlueprintCallable, Category="Reload")
	virtual void FinishReload();

public:
	const UWeaponData* GetWeaponData() const {return WeaponData;};
	
	/**
	 * @brief Setup/Breakdown on switch
	 */
	bool Equip();
	void UnEquip() const;
	void Drop();

	////////////////////
    ///////	World
    ////////////////////
public:
	UFUNCTION()
	virtual void Interact(APlayerController* PlayerController) override;
	
	
	////////////////////
	/////// Getters
	////////////////////
	/**
	 * @brief Gets the current ammo counts for this weapon.
	 */
	int GetMagAmmo() const {return CurrentMagAmmo;}
};