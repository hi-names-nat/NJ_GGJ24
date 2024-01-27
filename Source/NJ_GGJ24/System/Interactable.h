// (c) Dark Frog SGS 2023.

#pragma once

#include "CoreMinimal.h"
#include "InteractInterface.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class NJ_GGJ24_API AInteractable : public AActor, public IInteractInterface 
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnInteracted(AController* InteractInstigator);

	virtual void Interact(APlayerController* PlayerController) override {OnInteracted(PlayerController);};
};
