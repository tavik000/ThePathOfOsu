// Fill out your copyright notice in the Description page of Project Settings.


#include "CompleteAllMissionGameMode.h"

#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ACompleteAllMissionGameMode::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnPlayerDeath.AddDynamic(this, &ACompleteAllMissionGameMode::OnPlayerDeath);
	}
}

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


	UGameplayStatics::SpawnSound2D(GetWorld(), CompleteMissionSound);
	if (IsAllMissionCompleted())
	{
		WinGame();
	}
}

void ACompleteAllMissionGameMode::OnPlayerDeath()
{
	LoseGame();
}

void ACompleteAllMissionGameMode::LoseGame()
{
	GetWorldTimerManager().SetTimer(
		LoseGameTimer, this, &ACompleteAllMissionGameMode::ShowLoseScreen, 2.0f, false);
}

void ACompleteAllMissionGameMode::ShowLoseScreen()
{
	if (!LoseScreenWidgetClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("LoseScreenWidgetClass is null")));
		return;
	}
	CreateWidget<UUserWidget>(GetWorld(), LoseScreenWidgetClass)->AddToViewport();

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
	PlayerController->GameHasEnded(PlayerController->GetPawn(), false);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);
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
	PlayerController->SetIgnoreMoveInput(true);
	GetWorldTimerManager().SetTimer(
		WinGameTimer, this, &ACompleteAllMissionGameMode::ShowWinScreen, 2.0f, false);

}

void ACompleteAllMissionGameMode::ShowWinScreen()
{
	if (!WinScreenWidgetClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("WinScreenWidgetClass is null")));
		return;
	}
	CreateWidget<UUserWidget>(GetWorld(), WinScreenWidgetClass)->AddToViewport();

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
	PlayerController->GameHasEnded(PlayerController->GetPawn(), true);
	
}
