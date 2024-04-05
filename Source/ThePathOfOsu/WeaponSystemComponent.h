// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pistol.h"
#include "Rifle.h"
#include "Components/ActorComponent.h"
#include "WeaponSystemComponent.generated.h"

class AOxCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEPATHOFOSU_API UWeaponSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponSystemComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TryFire();
	void OnFireActionEnd();

	UFUNCTION(BlueprintCallable)
	void UnequipAllWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipRifle();

	UFUNCTION(BlueprintCallable)
	void UnequipRifle();

	UFUNCTION(BlueprintCallable)
	void EquipPistol();

	UFUNCTION(BlueprintCallable)
	void UnequipPistol();

private:
	AOxCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	FName PistolHandSocketName;

	UPROPERTY(EditAnywhere)
	FName PistolHostSocketName;

	UPROPERTY(EditAnywhere)
	FName RifleHandSocketName;

	UPROPERTY(EditAnywhere)
	FName RifleJogSocketName;

	UPROPERTY(EditAnywhere)
	FName RifleHostSocketName;

	APistol* Pistol;
	ARifle* Rifle;
};
