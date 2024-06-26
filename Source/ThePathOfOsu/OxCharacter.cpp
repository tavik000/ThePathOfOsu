// Fill out your copyright notice in the Description page of Project Settings.


#include "OxCharacter.h"
#include "ThePathOfOsuGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AOxCharacter::AOxCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LeftFistCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFistCollisionComponent"));
	LeftFistCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                              TEXT("hand_l"));
	LeftFistCollisionComponent->SetSphereRadius(32.0f);
	RightFistCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RightFistCollisionComponent"));
	RightFistCollisionComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                               TEXT("hand_r"));
	RightFistCollisionComponent->SetSphereRadius(32.0f);

	WeaponSystemComponent = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystemComponent"));

	PistolSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PistolSceneComponent"));
	PistolSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                        TEXT("PistolHost_Socket"));
	PistolChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("PistolChildActorComponent"));
	PistolChildActorComponent->
		AttachToComponent(PistolSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);


	RifleSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RifleSceneComponent"));
	RifleSceneComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,
	                                       TEXT("RifleHost_Socket"));
	RifleChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("RifleChildActorComponent"));
	RifleChildActorComponent->AttachToComponent(RifleSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);


	CurrentHp = MaxHp;
	DefaultCapsuleRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
}

// Called when the game starts or when spawned
void AOxCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();
	CharacterMovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());

	LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFistCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AOxCharacter::OnOverlapBegin);
	RightFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFistCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AOxCharacter::OnOverlapBegin);

	AnimInstance->OnMontageEnded.AddDynamic(this, &AOxCharacter::OnMontageEnded);
	CurrentHp = MaxHp;
	CurrentPostureValue = MaxPostureValue;
}


float AOxCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	if (IsDead())
	{
		return 0;
	}

	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(CurrentHp, DamageToApply);
	CurrentHp -= DamageToApply;

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White,
	//                                  FString::Printf(
	// 	                                 TEXT("%s Take Damage %f, remain HP %f"), *GetName(), DamageToApply,
	// 	                                 CurrentHp));
	if (IsPushing())
	{
		OnInterruptPushing.Broadcast();
	}

	if (IsAlive())
	{
		if (!AnimInstance->Montage_IsPlaying(BreakMontage) && !Cast<AGunBase>(DamageCauser))
		{
			PlayAnimMontage(HitReactMontage);
		}
	}
	else
	{
		Die();
	}
	return DamageToApply;
}

void AOxCharacter::Heal(float HealAmount)
{
	if (HealAmount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("Hp To Restore <= 0 %f"), HealAmount));
		return;
	}
	CurrentHp = FMath::Min(MaxHp, CurrentHp + HealAmount);
}

void AOxCharacter::ReducePostureValue(float PostureValueToReduce)
{
	CurrentPostureValue = FMath::Max(0.0f, CurrentPostureValue - PostureValueToReduce);
	if (CurrentPostureValue <= 0)
	{
		BreakPosture();
	}
}

void AOxCharacter::RestorePostureValue(float PostureValueToRestore)
{
	CurrentPostureValue = FMath::Min(MaxPostureValue, CurrentPostureValue + PostureValueToRestore);
}

void AOxCharacter::BreakPosture()
{
	if (!BreakMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("BreakMontage is null! %s"), *GetName());
		return;
	}
	if (IsDead())
	{
		return;
	}

	IsExecutable = true;

	if (!AnimInstance->Montage_IsPlaying(BreakMontage))
	{
		AnimInstance->StopAllMontages(0.0f);
		PlayAnimMontage(BreakMontage, BreakMontagePlayRate);
	}
}

void AOxCharacter::RestorePostureFromBreak()
{
	if (IsExecutable)
	{
		IsExecutable = false;
		RestorePostureValue(RecoverFromBreakPostureValue);
	}
}

bool AOxCharacter::CanRegenPosture()
{
	return !AnimInstance->Montage_IsPlaying(BeReflectedMontage)
		&& !AnimInstance->Montage_IsPlaying(BreakMontage)
		&& !AnimInstance->Montage_IsPlaying(HitReactMontage) && !AnimInstance->Montage_IsPlaying(DieMontage)
		&& !AnimInstance->Montage_IsPlaying(BlockMontage)
		&& IsAlive()
		&& BlockMovementReasons.IsEmpty() && !IsJumping();
}

bool AOxCharacter::CanMove()
{
	return !AnimInstance->Montage_IsPlaying(BeReflectedMontage)
		&& !AnimInstance->Montage_IsPlaying(BreakMontage)
		&& !AnimInstance->Montage_IsPlaying(HitReactMontage) && !AnimInstance->Montage_IsPlaying(DieMontage) &&
		!AnimInstance->Montage_IsPlaying(ExecutePunchAttackMontage) && !AnimInstance->Montage_IsPlaying(BlockMontage)
		&& IsAlive()
		&& !IsPlayingFistAttackMontage()
		&& BlockMovementReasons.IsEmpty();
}

bool AOxCharacter::CanCharacterJump()
{
	return IsAlive() && !AnimInstance->Montage_IsPlaying(BeReflectedMontage) &&
		!AnimInstance->Montage_IsPlaying(BreakMontage) &&
		!AnimInstance->Montage_IsPlaying(HitReactMontage) &&
		!AnimInstance->Montage_IsPlaying(ExecutePunchAttackMontage) && !AnimInstance->Montage_IsPlaying(BlockMontage)
		&& !IsPlayingFistAttackMontage()
		&& BlockMovementReasons.IsEmpty();
}

bool AOxCharacter::CanAttack()
{
	return IsAlive() && !AnimInstance->Montage_IsPlaying(BeReflectedMontage) &&
		!AnimInstance->Montage_IsPlaying(BreakMontage) &&
		!AnimInstance->Montage_IsPlaying(HitReactMontage) &&
		!AnimInstance->Montage_IsPlaying(ExecutePunchAttackMontage) && !AnimInstance->Montage_IsPlaying(BlockMontage)
		&& BlockMovementReasons.IsEmpty() && !IsJumping() && !IsDodging() && !IsPushing();
}

bool AOxCharacter::CanPunch()
{
	return true;
}

bool AOxCharacter::CanFire()
{
	return true;
}

bool AOxCharacter::IsMoving()
{
	return CharacterMovementComponent->Velocity.Size() > 0.3f || CharacterMovementComponent->IsFalling() ||
		!CharacterMovementComponent->GetCurrentAcceleration().Equals(FVector::ZeroVector, 0.1f);
}

bool AOxCharacter::IsPlayingFistAttackMontage()
{
	for (UAnimMontage* FistAttackMontage : FistAttackMontages)
	{
		if (AnimInstance->Montage_IsPlaying(FistAttackMontage))
		{
			return true;
		}
	}
	return false;
}

bool AOxCharacter::CanGuard()
{
	return IsAlive() && !AnimInstance->Montage_IsPlaying(BeReflectedMontage) &&
		!AnimInstance->Montage_IsPlaying(BreakMontage) &&
		!AnimInstance->Montage_IsPlaying(HitReactMontage) &&
		!AnimInstance->Montage_IsPlaying(ExecutePunchAttackMontage)
		&& !AnimInstance->Montage_IsPlaying(BlockMontage)
		&& !IsPlayingFistAttackMontage() && !IsJumping()
	&& !IsDodging();
}

bool AOxCharacter::CanUseItem()
{
	return CanMove() && !IsJumping() && !IsDodging();
}

bool AOxCharacter::CanOsu()
{
	return CanMove() && !IsJumping() && !IsDodging();
}

bool AOxCharacter::CanDodgeRoll()
{
	return CanMove() && !IsJumping();
}

bool AOxCharacter::CanPush()
{
	return CanMove() && !AnimInstance->Montage_IsPlaying(PushMontage) && !IsJumping();
}

void AOxCharacter::Die()
{
	AnimInstance->StopAllMontages(0.0f);
	CharacterMovementComponent->StopMovementImmediately();
	CharacterMovementComponent->Deactivate();
	SetActorEnableCollision(false);
}

void AOxCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("Montage Ended %s"), *Montage->GetName()));
	if (!bInterrupted && Montage->GetName().Contains("Break"))
	{
		RestorePostureFromBreak();
	}
}

// Called every frame
void AOxCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanRegenPosture())
	{
		CurrentPostureValue = FMath::Min(MaxPostureValue, CurrentPostureValue + PostureValueRegenRate * DeltaTime);
	}
}

// Called to bind functionality to input
void AOxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AOxCharacter::TryGuard()
{
}

void AOxCharacter::TryAttack()
{
}

void AOxCharacter::TryFistAttack()
{
}

void AOxCharacter::BeginFistAttack(bool IsLeftFist)
{
	HittingActorList.Empty();
	if (IsLeftFist)
	{
		LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		RightFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AOxCharacter::EndFistAttack(bool IsLeftFist)
{
	if (IsLeftFist)
	{
		LeftFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		RightFistCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AOxCharacter::TryUseItem()
{
	if (!DrinkPotionMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("DrinkPotionMontage is null! %s"), *GetName());
		return;
	}
	// Vinegar Only
	if (!CanUseItem())
	{
		return;
	}

	AnimInstance->Montage_Play(DrinkPotionMontage, 1.0f);
}

void AOxCharacter::TryOsu()
{
	if (!OsuMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("OsuMontage is null! %s"), *GetName());
		return;
	}
	if (!CanOsu())
	{
		return;
	}

	AnimInstance->Montage_Play(OsuMontage, 1.0f);
	DoOsuGesture.Broadcast();
}

void AOxCharacter::TryDodgeRoll()
{
}

bool AOxCharacter::TryPush()
{
	if (!PushMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("PushMontage is null! %s"), *GetName());
		return false;
	}
	if (!CanPush())
	{
		return false;
	}

	UCapsuleComponent* PlayerCapsule = GetCapsuleComponent();
	PlayerCapsule->SetCapsuleRadius(PushingEnlargedCapsuleRadius);
	PlayerCapsule->MoveComponent(GetActorForwardVector() * PushMoveCapsuleOffset, PlayerCapsule->GetComponentRotation(),
	                             false);
	AnimInstance->Montage_Play(PushMontage, 1.0f);
	return true;
}

void AOxCharacter::EndPush()
{
	if (!PushMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("PushMontage is null! %s"), *GetName());
		return;
	}
	if (IsPushing())
	{
		AnimInstance->Montage_JumpToSection(FName("End"), PushMontage);
	}
	GetCapsuleComponent()->SetCapsuleRadius(DefaultCapsuleRadius);
}

bool AOxCharacter::IsPushing() const
{
	return AnimInstance->Montage_IsPlaying(PushMontage);
}

bool AOxCharacter::IsDodging()
{
	return AnimInstance->Montage_IsPlaying(DodgeRollForwardMontage) ||
		AnimInstance->Montage_IsPlaying(DodgeRollBackwardMontage) ||
		AnimInstance->Montage_IsPlaying(DodgeRollLeftMontage) ||
		AnimInstance->Montage_IsPlaying(DodgeRollRightMontage);
}

bool AOxCharacter::IsJumping()
{
	return CharacterMovementComponent->IsFalling();
}

void AOxCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("Overlap Begin %s"), *OtherActor->GetName());
	if (OtherActor && OtherActor != this && !HittingActorList.Contains(OtherActor))
	{
		HittingActorList.Add(OtherActor);

		AOxCharacter* VictimActor = Cast<AOxCharacter>(OtherActor);
		if (!VictimActor)
		{
			// UE_LOG(LogTemp, Error, TEXT("Cannot Cast to AOxCharacter, OnOverlapBegin, This: %s, OtherActor: %s, OtherComp: %s, OverlappedComponent: %s"),
			// 	*GetName(), *OtherActor->GetName(), *OtherComp->GetName(), *OverlappedComponent->GetName());
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Cannot Cast to AOxCharacter, OnOverlapBegin, This: %s, OtherActor: %s, OtherComp: %s, OverlappedComponent: %s"),
			// 	*GetName(), *OtherActor->GetName(), *OtherComp->GetName(), *OverlappedComponent->GetName()));
			return;
		}
		if (IsAttackReflectable && VictimActor->IsGuardReflectable)
		{
			// Reflect
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Reflect"));
			AnimInstance->StopAllMontages(0.0f);
			PlayAnimMontage(BeReflectedMontage, 1.0f);
			ReducePostureValue(VictimActor->PunchDamage * 5);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ReflectSound, GetActorLocation());
		}
		else
		{
			if (VictimActor->IsDodging())
			{
				return;
			}
			if (VictimActor->IsGuarding)
			{
				// Add Posture
				// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Blocking"));
				VictimActor->ReducePostureValue(PunchDamage);

				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), BlockSound, GetActorLocation());
			}
			else
			{
				// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("Damage"));

				AController* InstigatorController = GetInstigatorController();
				UClass* DamageTypeClass = UDamageType::StaticClass();
				UGameplayStatics::ApplyDamage(OtherActor, PunchDamage, InstigatorController, this, DamageTypeClass);
				VictimActor->ReducePostureValue(0.7f * PunchDamage);

				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PunchHitSound, GetActorLocation());
				if (HitEffectActor)
				{
					// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffectActor, SweepResult.ImpactPoint,
					// GetActorRotation());

					AActor* HitVfx = GetWorld()->SpawnActor<AActor>(HitEffectActor,
					                                                OverlappedComponent->GetComponentLocation(),
					                                                GetActorRotation());
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("HitEffect is null"));
				}
			}
		}
	}
}

bool AOxCharacter::IsDead() const
{
	return CurrentHp <= 0;
}

bool AOxCharacter::IsAlive() const
{
	return !IsDead();
}

float AOxCharacter::GetHpPercentage() const
{
	return CurrentHp / MaxHp;
}

float AOxCharacter::GetPostureValuePercentage() const
{
	return CurrentPostureValue / MaxPostureValue;
}

void AOxCharacter::SetTimeScale(float TimeScale)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeScale);
}

void AOxCharacter::OsuGestureRestorePosture()
{
	RestorePostureValue(OsuGestureRestorePostureAmount);
}

void AOxCharacter::SetAnimationState(EAnimationState NewAnimationState)
{
	CurrentAnimationState = NewAnimationState;
}

void AOxCharacter::PlayMontage(UAnimMontage* MontageToPlay, float PlayRate)
{
	AnimInstance->Montage_Play(MontageToPlay, PlayRate);
}

EAnimationState AOxCharacter::GetCurrentAnimationState() const
{
	return CurrentAnimationState;
}
