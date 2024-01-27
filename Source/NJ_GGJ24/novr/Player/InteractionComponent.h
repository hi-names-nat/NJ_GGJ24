// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


class UUBaseInteractableComponent;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NJ_GGJ24_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, meta=(AllowPrivateAccess=true))
	float InteractReach = 1000;

public:		
	// Sets default values for this component's properties
	UInteractionComponent();

	void AttemptInteract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
