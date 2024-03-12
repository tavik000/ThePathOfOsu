// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OxCharacter.h"
#include "Components/WidgetComponent.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
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
	
protected:
	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void TryFistAttack() override;

	virtual void BreakPosture() override;
	virtual void RestorePosture() override;
	virtual void Die() override;
private:
};
