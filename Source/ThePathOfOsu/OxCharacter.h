// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "OsuType.h"
#include "WeaponSystemComponent.h"
#include "OxCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginPush);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoOsuGesture);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInterruptPushing);

UCLASS()
class THEPATHOFOSU_API AOxCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOxCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UAnimInstance* AnimInstance;

	UCharacterMovementComponent* CharacterMovementComponent;


	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BlockMontagePlayRate = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TArray<UAnimMontage*> FistAttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* ExecutePunchAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* BeReflectedMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* BreakMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DrinkPotionMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* OsuMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* PushMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BreakMontagePlayRate = 0.7f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DodgeRollForwardMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DodgeRollBackwardMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DodgeRollLeftMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DodgeRollRightMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DieMontage;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USphereComponent* LeftFistCollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USphereComponent* RightFistCollisionComponent;

	TArray<AActor*> HittingActorList;

	UPROPERTY(EditAnywhere)
	USoundBase* PunchHitSound;

	UPROPERTY(EditAnywhere)
	USoundBase* BlockSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ReflectSound;

	UPROPERTY(EditAnywhere)
	UClass* HitEffectActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UWeaponSystemComponent* WeaponSystemComponent;

	UPROPERTY(EditDefaultsOnly)
	float PushingEnlargedCapsuleRadius = 65.0f;
	UPROPERTY(EditDefaultsOnly)
	float PushMoveCapsuleOffset = -50.0f;

	UPROPERTY(EditAnywhere)
	float OsuGestureRestorePostureAmount = 30.0f;



	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
	virtual void Heal(float HealAmount);

	virtual void ReducePostureValue(float PostureValueToReduce);
	virtual void RestorePostureValue(float PostureValueToRestore);


	virtual void BreakPosture();
	virtual void RestorePostureFromBreak();


	UFUNCTION(BlueprintPure)
	virtual bool CanRegenPosture();

	UFUNCTION(BlueprintPure)
	virtual bool CanMove();

	UFUNCTION(BlueprintPure)
	virtual bool CanCharacterJump();

	UFUNCTION(BlueprintPure)
	virtual bool CanAttack();

	UFUNCTION(BlueprintPure)
	bool IsPlayingFistAttackMontage();

	UFUNCTION(BlueprintPure)
	virtual bool CanGuard();

	UFUNCTION(BlueprintPure)
	virtual bool CanUseItem();

	UFUNCTION(BlueprintPure)
	virtual bool CanOsu();
	
	UFUNCTION(BlueprintPure)
	virtual bool CanDodgeRoll();


	virtual void Die();

	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	
	UPROPERTY(VisibleAnywhere)
	EAnimationState CurrentAnimationState;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintPure)
	virtual bool CanPush();

	virtual void TryGuard();
	virtual void TryAttack();
	virtual void TryFistAttack();
	virtual void BeginFistAttack(bool IsLeftFist);
	virtual void EndFistAttack(bool IsLeftFist);

	virtual void TryUseItem();
	virtual void TryOsu();
	virtual bool TryPush();
	virtual void TryDodgeRoll();
	void EndPush();
	bool IsPushing();
	bool IsDodging();
	bool IsJumping();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	float MaxHp = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float CurrentHp = MaxHp;

	UFUNCTION(BlueprintPure)
	float GetHpPercentage() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MaxPostureValue = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float CurrentPostureValue;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float PostureValueRegenRate = 5.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float RecoverFromBreakPostureValue = 30.0f;

	UFUNCTION(BlueprintPure)
	float GetPostureValuePercentage() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float PunchDamage = 15;

	bool IsAttackReflectable = false;
	bool IsGuardReflectable = false;
	bool IsGuarding = false;
	bool IsExecutable = false;

	TSet<FString> BlockMovementReasons;

	FOnBeginPush OnBeginPush;

	UPROPERTY(BlueprintAssignable)
	FDoOsuGesture DoOsuGesture;

	void SetTimeScale(float TimeScale);

	void OsuGestureRestorePosture();

	UPROPERTY(BlueprintAssignable)
	FOnInterruptPushing OnInterruptPushing;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PistolSceneComponent;

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* PistolChildActorComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RifleSceneComponent;

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* RifleChildActorComponent;


	UFUNCTION(BlueprintPure)
	EAnimationState GetCurrentAnimationState() const;

	UFUNCTION(BlueprintCallable)
	void SetAnimationState(EAnimationState NewAnimationState);

	

private:
	float DefaultCapsuleRadius;
	
};
