// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OxCharacter.h"
#include "Components/WidgetComponent.h"
#include "EnemyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyStartBattle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyEndBattle);

UCLASS()
class THEPATHOFOSU_API AEnemyCharacter : public AOxCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* TargetWidget;
	
	void ShowTargetWidget();
	void HideTargetWidget();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* ExecutableTargetWidget;
	
	void ShowExecutableTargetWidget();
	void HideExecutableTargetWidget();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthBarWidget;
	
	void HideHealthBar();

	UPROPERTY(BlueprintAssignable)
	FOnEnemyDeath OnEnemyDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AcquisitionRange = 500.0f;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEnemyStartBattle OnEnemyStartBattle;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnEnemyEndBattle OnEnemyEndBattle;
	
protected:
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void TryFistAttack() override;

	virtual void BreakPosture() override;
	virtual void RestorePostureFromBreak() override;
	virtual void Die() override;

	
private:
};
