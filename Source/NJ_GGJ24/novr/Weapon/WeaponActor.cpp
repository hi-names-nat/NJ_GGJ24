// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationSystemTypes.h"
#include "WeaponData.h"
#include "Ammunition/AmmoBackpack.h"
#include "Kismet/GameplayStatics.h"
#include "NJ_GGJ24/novr/Player/AsPlayer.h"
#include "NJ_GGJ24/System/PickupComponent.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->bEditableWhenInherited=true;
	WeaponWorldmodel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon Worldmodel"));
	WeaponWorldmodel->SetupAttachment(RootComponent);
	WeaponWorldmodel->bEditableWhenInherited=true;
	WeaponViewmodel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Viewmodel"));
	WeaponViewmodel->SetupAttachment(RootComponent);
	WeaponWorldmodel->SetSimulatePhysics(true);
	Interaction = CreateDefaultSubobject<UPickupComponent>(TEXT("Pickup interaction layer"));
	Interaction->SetupAttachment(WeaponWorldmodel);
	Interaction->OnPickUp.AddDynamic(this, &AWeaponActor::Interact);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	CurrentMagAmmo = InitialAmmo;

	if (IsFullAuto)
	{
		TimerDelegate.BindUObject(this, &AWeaponActor::FireLoop);
	}
	else
	{
		TimerDelegate.BindUObject(this, &AWeaponActor::ResetFireCooldown);
	}
	EndDelegate.BindUObject(this, &AWeaponActor::ResetFireCooldown);

	WeaponViewmodel->SetVisibility(false,true);
	
}

void AWeaponActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (Player == nullptr)
	{
		return;
	}

	if (const APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(MappingContext);
		}
	}
}

void AWeaponActor::BeginFire()
{
	FireHeld = true;
	Fire();
}

bool AWeaponActor::Fire()
{
	// bCanFire handles weapon cooldowns and the like.
	if (!bCanFire) return false;

	const bool hasAmmo = CurrentMagAmmo == 0;

	PlayWeaponSound(hasAmmo);

	if (!hasAmmo) return false;

	bCanFire = false;
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	
	//Different delegate for the last shot the player has, so looping can be prevented.
	if (CurrentMagAmmo != 0)
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TimeBetweenShots, false);
	else
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, EndDelegate, TimeBetweenShots, false);
	return true;
}

void AWeaponActor::PlayWeaponSound(bool hasAmmo) const
{
	USoundBase* SoundToPlay = hasAmmo ? FireSound : NoAmmoSound;
	switch(WeaponData->WeaponFireType)
	{

	case EWeaponFireType::Continuous:
		break;
	case EWeaponFireType::Discrete:
	default:
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, Player->GetActorLocation());
	}
}


void AWeaponActor::EndFire()
{
	FireHeld = false;
}

void AWeaponActor::FireLoop()
{
	bCanFire = true;
	if (!FireHeld) return;
	Fire();
}

void AWeaponActor::Reload_Implementation()
{
	if (Player->GetAmmoBackpack()->GetReserve(AmmoType) == 0) return;
	//Animate using blueprint
}

void AWeaponActor::FinishReload()
{
	CurrentMagAmmo = Player->GetAmmoBackpack()->GetAmmoOut(AmmoType, MagCapacity);
}

bool AWeaponActor::Equip()
{
	if (!BeginFireBinding || !EndFireBinding) return false;
	// Set up action bindings
	if (const APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			BeginFireBinding = &EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AWeaponActor::BeginFire);
			EndFireBinding = &EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AWeaponActor::EndFire);
			ReloadBinding = &EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Completed, this, &AWeaponActor::EndFire);
		}
	}
	WeaponViewmodel->SetVisibility(true, true);
	WeaponWorldmodel->SetVisibility(true, true);
	WeaponWorldmodel->SetSimulatePhysics(false);
	return true;
}

void AWeaponActor::UnEquip() const
{
	if (!BeginFireBinding || !EndFireBinding) return;
	// Disable action bindings
	if (const APlayerController* PlayerController = Cast<APlayerController>(Player->GetController()))
	{
		if (UEnhancedInputComponent* Subsystem = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			Subsystem->RemoveBindingByHandle(BeginFireBinding->GetHandle());
			Subsystem->RemoveBindingByHandle(EndFireBinding->GetHandle());
			Subsystem->RemoveBindingByHandle(ReloadBinding->GetHandle());
		}
	}

	WeaponViewmodel->SetVisibility(false, true);
	WeaponWorldmodel->SetVisibility(false, true);
}

void AWeaponActor::Drop()
{
	UnEquip();
	WeaponViewmodel->SetVisibility(false, true);
	WeaponWorldmodel->SetVisibility(true, true);
	WeaponWorldmodel->SetSimulatePhysics(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Owner = nullptr;
	isOwned=false;
}

void AWeaponActor::Interact(APlayerController* PlayerController)
{
	if (isOwned)return;
	if (auto PlayerCharacter = Cast<AAsPlayer>(PlayerController->GetPawn()))
	{
		PlayerCharacter->SetWeaponInSlot(this);
		AttachToActor(PlayerCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GunRoot");
		Owner = PlayerCharacter;
		WeaponWorldmodel->bOwnerNoSee = true;
		WeaponViewmodel->bOnlyOwnerSee = true;
		isOwned=true;
	}
}

