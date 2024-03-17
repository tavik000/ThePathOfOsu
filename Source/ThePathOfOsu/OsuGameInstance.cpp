// Fill out your copyright notice in the Description page of Project Settings.


#include "OsuGameInstance.h"

#include "PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UOsuGameInstance::TogglePauseGame()
{
	if (!PauseMenuWidgetClass)
	{
		GetEngine()->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "PauseMenuWidgetClass is null");
		return;
	}
	if (IsGamePaused)
	{
		OnGameResume.Broadcast();
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		// PlayerCharacter->EnableInput(PlayerController);
		IsGamePaused = false;
	}
	else
	{
		if (!PlayerController)
		{
			PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		}
		if (!PlayerCharacter)
		{
			PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		}
		PauseMenuWidget = CreateWidget<UUserWidget>(PlayerController, PauseMenuWidgetClass);
		PauseMenuWidget->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		// PlayerController->SetInputMode(FInputModeUIOnly());

		// PlayerCharacter->DisableInput(PlayerController);
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeGameAndUI());
		IsGamePaused = true;
	}
}
