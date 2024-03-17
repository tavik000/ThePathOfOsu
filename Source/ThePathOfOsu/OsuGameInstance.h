// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OsuGameInstance.generated.h"

/**
 * Handle Save Game And Pause Menu
 */

class APlayerCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameResume);

UCLASS()
class THEPATHOFOSU_API UOsuGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseGame();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UUserWidget* PauseMenuWidget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsGamePaused;

	UPROPERTY(BlueprintAssignable)
	FOnGameResume OnGameResume;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float MusicVolume = 100.0f;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float SFXVolume = 100.0f;


private:
	APlayerController* PlayerController;
	APlayerCharacter* PlayerCharacter;
};
