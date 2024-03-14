// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPawn.generated.h"

UCLASS()
class THEPATHOFOSU_API AMainMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	AMainMenuPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	UUserWidget* MainMenuWidget;

	// TODO gamepad UI controls
	
private:
	APlayerController* PlayerController;
};
