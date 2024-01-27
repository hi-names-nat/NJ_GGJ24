// (c) Dark Frog SGS 2023.


#include "AsCharacter.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AAsCharacter::AAsCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	HealthComponent->SetupAttachment(RootComponent);
	HealthComponent->OnKilled.AddDynamic(this, &AAsCharacter::OnDie);
	HealthComponent->OnHurt.AddDynamic(this, &AAsCharacter::OnHurt);
}

// Called when the game starts or when spawned
void AAsCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAsCharacter::SetHealth(int Amount)
{
	HealthComponent->SetHealth(Amount);
}

void AAsCharacter::RecoverHealth(int Amount)
{
	HealthComponent->HealDamage(Amount);
}

void AAsCharacter::DealHealth(int Amount)
{
	HealthComponent->TakeDamage(Amount);
}

void AAsCharacter::OnHurt_Implementation()
{
	AudioComponent->Stop();
	PlayAudioFromComponent(DamageSound);
}

void AAsCharacter::OnDie_Implementation()
{
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AudioComponent->Stop();
	PlayAudioFromComponent(DieSound);
}

// Called every frame
void AAsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AAsCharacter::PlayAudioFromComponent(TObjectPtr<USoundBase> SoundCue)
{
	if (AudioComponent == nullptr || AudioComponent->IsPlaying()) return false;
	AudioComponent->Sound = SoundCue;
	AudioComponent->Play();
	return true;
}



