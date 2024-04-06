// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "OxCharacter.h"
#include "Logging/LogMacros.h"
#include "Item.h"
#include "OsuType.h"
#include "OsuGameInstance.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerUseItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunZoomIn);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGunZoomOut);

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
	UInputAction* GuardOrZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TargetLockAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseItemAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeRollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OsuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

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
	virtual bool CanOsu() override;
	bool CanCrouch();
	bool CanUncrouch();
	bool CanSprint();
	virtual bool CanAttack() override;
	virtual bool CanPunch() override;
	virtual bool CanFire() override;
	virtual bool CanGuard() override;
	virtual bool CanCharacterJump() override;

	void OnSprintStart();

	void OnSprintEnd();

	void TryCrouch();

	virtual void TryDodgeRoll() override;

	void OnAttackActionEnd();
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted) override;
	virtual void Die() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	virtual void TryAttack() override;
	bool IsMoveInputBeingPressed();
	virtual void TryGuard() override;
	void TryGuardOrZoom();
	void TryZoomOut();
	void GunZoomInCamera();
	void GunZoomOutCamera();
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

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	UItem* CurrentSlotItem;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetInventoryItemCount(UItem* Item) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool HasItem(UItem* Item);

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

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath OnPlayerDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed = 150.0f;

	UPROPERTY(BlueprintReadOnly)
	float TurnRate;


	UFUNCTION(BlueprintCallable)
	void ShowCrosshair();

	UFUNCTION(BlueprintCallable)
	void HideCrosshair();

private:
	float WalkSpeed;

	bool IsMeleeAttackInputReceived = false;

	bool IsMovingInputPressing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsCrouching = false;

	UPROPERTY(EditDefaultsOnly)
	float MaxStamina = 100;
	UPROPERTY(VisibleAnywhere)
	float CurrentStamina;
	UPROPERTY(EditDefaultsOnly)
	float StaminaRegenRate = 10;
	UPROPERTY(EditDefaultsOnly)
	float RollStaminaCost = 70;
	UPROPERTY(EditDefaultsOnly)
	float FindHighlightInteractiveObjectDistance = 350.0f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsTargetLocking = false;
	AEnemyCharacter* LockTargetEnemy = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceEnemyObjectTypes;


	APlayerController* PlayerController;
	AEnemyCharacter* ExecutingTarget = nullptr;

	FTimerHandle FindInteractableTimerHandle;
	void FindAndHighlightInteractableObjectNearPlayer();

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> InteractableObjectTypes;

	void TogglePauseGame();

	UOsuGameInstance* GameInstance;

	FVector2D CurrentMovementVector;

	float DefaultTargetArmLength;;

	void SetupGunCameraZoomTimeline();

	FTimeline GunCameraZoomTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* GunCameraZoomCurve;

	UPROPERTY(EditDefaultsOnly)
	FVector GunZoomCameraSocketOffset;

	UPROPERTY(EditDefaultsOnly)
	FVector CrouchGunZoomCameraSocketOffset;

	FVector DefaultCameraSocketOffset;

	UPROPERTY(EditDefaultsOnly)
	float GunZoomTargetArmLength;

	UPROPERTY(EditDefaultsOnly)
	float CrouchGunZoomTargetArmLength;

	UFUNCTION()
	void GunCameraZoomTimelineProgress(float Value);

	bool IsGunZooming = false;

	UPROPERTY(BlueprintAssignable)
	FOnGunZoomIn OnGunZoomIn;

	UPROPERTY(BlueprintAssignable)
	FOnGunZoomOut OnGunZoomOut;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UUserWidget* CrosshairWidget;
	void SetupCrosshairWidget();
};
