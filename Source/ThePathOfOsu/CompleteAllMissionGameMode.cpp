// Fill out your copyright notice in the Description page of Project Settings.


#include "CompleteAllMissionGameMode.h"

void ACompleteAllMissionGameMode::CompleteMission(UOsuMission* Mission)
{
	if (CurrentMission.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentMission is empty, Function name: %s"), *FString(__FUNCTION__));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("CurrentMission is empty, Function name: %s"),
			                                 *FString(__FUNCTION__)));
		return;
	}
	if (!Mission)
	{
		UE_LOG(LogTemp, Error, TEXT("Mission is null, Function name: %s"), *FString(__FUNCTION__));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("Mission is null, Function name: %s"), *FString(__FUNCTION__)));
		return;
	}
	if (!CurrentMission.Contains(Mission))
	{
		UE_LOG(LogTemp, Error, TEXT("Mission is not in the map, Function name: %s"), *FString(__FUNCTION__));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("Mission is not in the map, Function name: %s"),
			                                 *FString(__FUNCTION__)));
		return;
	}

	CurrentMission[Mission] = true;
	if (IsAllMissionCompleted())
	{
		WinGame();
	}
}

bool ACompleteAllMissionGameMode::IsAllMissionCompleted()
{
	for (auto& Mission : CurrentMission)
	{
		if (!Mission.Value)
		{
			return false;
		}
	}
	return true;
}

void ACompleteAllMissionGameMode::WinGame()
{
	// TODO
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("Win Game")));
	
}
