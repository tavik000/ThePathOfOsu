// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OsuMission.h"
#include "ThePathOfOsuGameMode.h"
#include "CompleteAllMissionGameMode.generated.h"

UCLASS()
class THEPATHOFOSU_API ACompleteAllMissionGameMode : public AThePathOfOsuGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	TMap<UOsuMission*, bool> CurrentMission;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void CompleteMission(UOsuMission* Mission);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	USoundBase* CompleteMissionSound;

	UFUNCTION(BlueprintCallable)
	void OnPlayerDeath();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	USoundBase* OsuSong;

private:
	void WinGame();
	void LoseGame();
	bool IsAllMissionCompleted();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WinScreenWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoseScreenWidgetClass;

	FTimerHandle LoseGameTimer;;
	FTimerHandle WinGameTimer;
	APlayerController* PlayerController;
	void ShowWinScreen();
	void ShowLoseScreen();
};
