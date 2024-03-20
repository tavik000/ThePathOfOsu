// Fill out your copyright notice in the Description page of Project Settings.

#include "PenLight.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


APenLight::APenLight()
{
	PrimaryActorTick.bCanEverTick = true;
	HasBeenRepaired = false;
}

void APenLight::BeginPlay()
{
	Super::BeginPlay();
	if (!SubtitleActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("SubtitleActor is null!, function: ALiveTrigger::BeginPlay()")));
	}
}

void APenLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APenLight::Interact_Implementation(APlayerCharacter* InteractCharacter)
{
	if (CanPickup(InteractCharacter))
	{
		IInteractableInterface::Interact_Implementation(InteractCharacter);
		OnInteract.Broadcast();
		GiveItem();
	}
	else
	{
		if (!HasBeenRepaired)
		{
			if (!CanRepair(InteractCharacter))
			{
				SubtitleActor->ShowSubtitleFromScript(SubtitleText.ToString(), SubtitleDuration);
			}
			else
			{
				Repair();
				OnInteract.Broadcast();
				InteractCharacter->RemoveInventoryItem(RequireItemType);
			}
		}
	}
}

bool APenLight::CanPickup(APlayerCharacter* PickingCharacter)
{
	return HasBeenRepaired && Super::CanPickup(PickingCharacter);
}

bool APenLight::CanRepair(APlayerCharacter* PickingCharacter)
{
	if (!RequireItemType)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("RequireItemType is null! function: APenLight::CanPickup()")));
		return false;
	}
	return PickingCharacter->HasItem(RequireItemType) && Super::CanPickup(PickingCharacter);
}

void APenLight::Repair()
{
	Mesh->SetMaterial(0, LightingMaterial);
	InteractionHUD->SetSprite(PickupIcon);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RepairSound, GetActorLocation());
	HasBeenRepaired = true;
}
