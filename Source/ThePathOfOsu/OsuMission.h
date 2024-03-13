// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "OsuMission.generated.h"

UCLASS()
class THEPATHOFOSU_API UOsuMission : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UOsuMission()
		: CountToComplete(1)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	FText MissionId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	FText MissionName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mission")
	int32 CountToComplete;
};
