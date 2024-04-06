// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* PistolFireMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RifleFireMontage;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TryFire();
	void OnFireActionEnd();

	void PlayFireMontage();

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

	void StartSprint();
	void EndSprint();

	UFUNCTION()
	void OnPlayerAddItem(UItem* Item);
	
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

	FTimerHandle RifleFireTimerHandle;
	void CheckRifleFire();

	bool IsRifleFiring = false;

	UPROPERTY(EditAnywhere)
	float RifleFireRate = 0.1f;
};
