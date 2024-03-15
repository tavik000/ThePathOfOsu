// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetArrayLibrary.h"

AEnemyCharacter::AEnemyCharacter()
{
	TargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TargetWidget"));
	TargetWidget->SetupAttachment(GetRootComponent());
	TargetWidget->SetWidgetSpace(EWidgetSpace::World);
	TargetWidget->SetDrawSize(FVector2D(50, 50));
	TargetWidget->SetGenerateOverlapEvents(false);
	TargetWidget->SetHiddenInGame(true);

	ExecutableTargetWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ExecutableTargetWidget"));
	ExecutableTargetWidget->SetupAttachment(GetRootComponent());
	ExecutableTargetWidget->SetWidgetSpace(EWidgetSpace::World);
	ExecutableTargetWidget->SetDrawSize(FVector2D(50, 50));
	ExecutableTargetWidget->SetGenerateOverlapEvents(false);
	ExecutableTargetWidget->SetHiddenInGame(true);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
}

void AEnemyCharacter::ShowTargetWidget()
{
	TargetWidget->SetHiddenInGame(false);
}

void AEnemyCharacter::HideTargetWidget()
{
	TargetWidget->SetHiddenInGame(true);
}

void AEnemyCharacter::ShowExecutableTargetWidget()
{
	ExecutableTargetWidget->SetHiddenInGame(false);
}

void AEnemyCharacter::HideExecutableTargetWidget()
{
	ExecutableTargetWidget->SetHiddenInGame(true);
}

void AEnemyCharacter::HideHealthBar()
{
	HealthBarWidget->SetHiddenInGame(true);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                  AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyCharacter::TryFistAttack()
{
	if (!AnimInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Enemy AnimInstance is null"));
		return;
	}


	int32 RandomIndex = FMath::RandRange(0, FistAttackMontages.Num() - 1);
	UAnimMontage* RandomFistAttackMontage = FistAttackMontages[RandomIndex];
	AnimInstance->Montage_Play(RandomFistAttackMontage, 1.0f);
}

void AEnemyCharacter::BreakPosture()
{
	Super::BreakPosture();
	ShowExecutableTargetWidget();
}

void AEnemyCharacter::RestorePostureFromBreak()
{
	Super::RestorePostureFromBreak();
	HideExecutableTargetWidget();
}

void AEnemyCharacter::Die()
{
	Super::Die();
	HideHealthBar();
	HideExecutableTargetWidget();
	OnEnemyDeath.Broadcast();
}
