// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPawn.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


AMainMenuPawn::AMainMenuPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuWidgetClass is null, Function name: %s"), *FString(__FUNCTION__));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("MainMenuWidgetClass is null, Function name: %s"),
			                                 *FString(__FUNCTION__)));
		return;
	}
	MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
}

void AMainMenuPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainMenuPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
