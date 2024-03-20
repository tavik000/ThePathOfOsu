// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Subtitle.h"
#include "PenLight.generated.h"

UCLASS()
class THEPATHOFOSU_API APenLight : public APickup
{
	GENERATED_BODY()

public:
	APenLight();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APlayerCharacter* InteractCharacter) override;
	virtual bool CanPickup(APlayerCharacter* PickingCharacter) override;
	bool CanRepair(APlayerCharacter* PickingCharacter);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItem *RequireItemType;

	UPROPERTY(EditAnywhere)
	ASubtitle* SubtitleActor;

	UPROPERTY(EditAnywhere)
	FText SubtitleText;

	UPROPERTY(EditDefaultsOnly)
	float SubtitleDuration = 4.0f;
	
	UPROPERTY(EditDefaultsOnly)
	UMaterial* LightingMaterial;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> PickupIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool HasBeenRepaired;

	UFUNCTION(BlueprintCallable)
	void Repair();
	
	UPROPERTY(EditAnywhere)
	USoundBase* RepairSound;
	
};
