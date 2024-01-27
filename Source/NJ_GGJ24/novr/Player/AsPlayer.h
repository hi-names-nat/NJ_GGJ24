// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "NJ_GGJ24/System/Character/AsCharacter.h"
#include "AsPlayer.generated.h"

class UWeaponComponent;
class AWeaponActor;
class UAmmoBackpack;
struct FEnhancedInputActionEventBinding;
class UInputMappingContext;
class UInputAction;
enum struct EWeaponType: uint8;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UInteractionComponent;


UCLASS(config=Game)
class NJ_GGJ24_API AAsPlayer : public AAsCharacter
{
	GENERATED_BODY()
////////////////////////////////////
/// Member Variables 
////////////////////////////////////
	////////////////////
	///////	Character Setup
	////////////////////
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Weapons, meta = (AllowPrivateAccess = "true"))
	USceneComponent* GunRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	AWeaponActor* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	AWeaponActor* ReserveWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	UAmmoBackpack* AmmoBackpack;

	////////////////////
	///////	Input 
	////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Mapping Context", Category="Input", meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Jump", Category="Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Move", Category="Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Swap Weapon", Category="Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* SwapWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName="Interact", Category="Input Actions", meta=(AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

protected:
	
	///////////////////
	///////	Debug 
	///////////////////


public:
	
////////////////////////////////////
/// Member Methods 
////////////////////////////////////

	////////////////////
	/////// Setup/Teardown
	////////////////////
public:
	AAsPlayer();
	virtual void Jump() override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:

	////////////////////
	///////	Weapons
	////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons)
	bool bHasWeapon;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapons)
	void SetHasWeapon(AWeaponActor* Weapon);

	// /** Getter for the bool */
	// UFUNCTION(BlueprintCallable, Category = Weapons)
	// bool GetHasWeapon(EWeaponType Weapon) const;

	UFUNCTION(BlueprintCallable, Category = Weapons)
	int GetAmmoForWeapon(bool bDesireReserveCount=false);

	///Assuming we're adding to current weapon
	UFUNCTION(BlueprintCallable, Category = Weapons)
	bool AddAmmoToWeapon(int Ammo);

	UFUNCTION(BlueprintCallable, Category=Weapons)
	void SwapWeapon();

	bool HasEmptySlot() const;

	void SetWeaponInSlot(AWeaponActor* newWeapon);

	////////////////////
	///////	Input
	////////////////////
protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	////////////////////
	/////// Getters
	////////////////////
public:
	/** Returns Mesh1P subobject **/
	USceneComponent* GetGunRoot() const {return GunRoot;}
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const {return FirstPersonCameraComponent;}

	UCameraComponent* GetCamera() const {return FirstPersonCameraComponent;}

	UAmmoBackpack* GetAmmoBackpack() const {return AmmoBackpack;}
	
};

