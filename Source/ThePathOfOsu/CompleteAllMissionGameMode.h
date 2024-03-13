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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	TMap<UOsuMission*, bool> CurrentMission;

	void CompleteMission(UOsuMission* Mission);
	

private:
	void WinGame();
	bool IsAllMissionCompleted();
};
