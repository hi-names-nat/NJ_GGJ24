// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractionComponent.h"
#include "NJ_GGJ24/novr/Weapon/WeaponActor.h"


//////////////////////////////////////////////////////////////////////////
// AAtomic2Character

AAsPlayer::AAsPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	bHasWeapon = false;
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	GunRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Gun Root"));
	GunRoot->SetupAttachment(FirstPersonCameraComponent);
	GunRoot->SetRelativeLocation(FVector(100.f, 0.f, 0.f));
	
	Interaction = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interact"));
	
	CurrentWeapon = nullptr;
}

void AAsPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	GunRoot->SetVisibility(false, true);
}

//////////////////////////////////////////////////////////////////////////// Input

void AAsPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAsPlayer::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAsPlayer::Look);

		//Weapon Swap
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Completed, this, &AAsPlayer::SwapWeapon);

		//Interaction
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, Interaction, &UInteractionComponent::AttemptInteract);

	}
}


void AAsPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	UE_LOG(LogTemp, Log, TEXT("HELLO THIS IS %s"), *MovementVector.ToString());

	if (Controller != nullptr)
	{
		// add movement
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
		AddMovementInput(GetActorRightVector(), MovementVector.Y);
	} 
}

void AAsPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAsPlayer::SetHasWeapon(AWeaponActor* Weapon)
{
	CurrentWeapon->UnEquip();
	CurrentWeapon = Weapon;
	Weapon->Equip();
}

// bool AAsPlayer::GetHasWeapon(const EWeaponType Weapon) const
// {
// 	return Weapon == CurrentWeapon->WeaponType || Weapon == ReserveWeapon->WeaponType;
// }

int AAsPlayer::GetAmmoForWeapon(bool bDesireReserveCount)
{
	//Awaiting implementation of a backpack ammo system
	UE_LOG(LogTemp, Error, TEXT("This function, AAsPlayer::GetAmmoForWeapon is not implemented!"));
	return true;
}

bool AAsPlayer::AddAmmoToWeapon(const int Ammo)
{
	//Awaiting implementation of a backpack ammo system
	UE_LOG(LogTemp, Error, TEXT("This function, AAsPlayer::GetAmmoForWeapon is not implemented!"));
	return true;
}

void AAsPlayer::SwapWeapon()
{
	if (CurrentWeapon == nullptr || ReserveWeapon == nullptr) return;
	CurrentWeapon->UnEquip();
	ReserveWeapon->Equip();
	Swap(CurrentWeapon, ReserveWeapon);
}

bool AAsPlayer::HasEmptySlot() const
{
	return ReserveWeapon==nullptr;
}

void AAsPlayer::SetWeaponInSlot(AWeaponActor* newWeapon)
{
	if (newWeapon == nullptr) return;
	if (ReserveWeapon!=nullptr) CurrentWeapon = newWeapon;
	else
	{
		ReserveWeapon = newWeapon;
		SwapWeapon();
	};
}

void AAsPlayer::Jump()
{
	Super::Jump();
	// VisualJump();
}
