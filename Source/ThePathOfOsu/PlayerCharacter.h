// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "OxCharacter.h"
#include "Logging/LogMacros.h"
#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerUseItem);

UCLASS(Config=Game)
class APlayerCharacter : public AOxCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseItemAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OsuAction;

public:
	APlayerCharacter();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void OnMoveActionStart();
	void OnMoveActionRelease();

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void SearchEnemyInFront(FHitResult& OutHit, bool& IsHit);
	void UnlockTarget();

	void TryJump();
	void Interact();
	virtual bool CanUseItem() override;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	bool IsMoveInputBeingPressed();
	virtual void TryBlock() override;
	virtual void TryFistAttack() override;
	void TryTargetLock();

	UFUNCTION()
	void OnPlayMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);


	virtual void BeginFistAttack(bool IsLeftFist) override;
	virtual void EndFistAttack(bool IsLeftFist) override;

	TArray<AActor*> CloseActors;
	AActor* FocusActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<UItem*, int32> InventoryData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UItem* CurrentSlotItem;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetInventoryItemCount(UItem* Item) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddInventoryItem(UItem* NewItem, int32 ItemCount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveInventoryItem(UItem* RemovedItem, int32 RemoveCount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseSlotItem();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerUseItem OnPlayerUseItem;
	
private:
	bool IsMeleeAttackInputReceived = false;

	bool IsMovingInputPressing = false;

	UPROPERTY(EditDefaultsOnly)
	float MaxStamina = 100;
	UPROPERTY(VisibleAnywhere)
	float CurrentStamina;
	UPROPERTY(EditDefaultsOnly)
	float StaminaRegenRate = 10;
	UPROPERTY(EditDefaultsOnly)
	float RollStaminaCost = 70;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsTargetLocking = false;
	AEnemyCharacter* LockTargetEnemy = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;


	APlayerController* PlayerController;
	AEnemyCharacter* ExecutingTarget = nullptr;

	FTimerHandle FindInteractableTimerHandle;
	void FindAndHighlightInteractableObjectNearPlayer();
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> InteractableObjectTypes;
};
