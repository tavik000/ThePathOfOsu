// Fill out your copyright notice in the Description page of Project Settings.


#include "LiveTrigger.h"

#include "PlayerCharacter.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"


ALiveTrigger::ALiveTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	IsActivated = false;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);

	InteractionHUD = CreateDefaultSubobject<UBillboardComponent>(TEXT("InteractionHUD"));
	InteractionHUD->SetupAttachment(RootComp);
	InteractionHUD->SetRelativeLocation(FVector(0.0f, -48.0f, 31.0f));
	InteractionHUD->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	InteractionHUD->SetHiddenInGame(false);
}

void ALiveTrigger::BeginPlay()
{
	Super::BeginPlay();

	InteractionHUD->SetVisibility(false);
	if (!SubtitleActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("SubtitleActor is null!, function: ALiveTrigger::BeginPlay()")));
	}
}

void ALiveTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALiveTrigger::Interact_Implementation(APlayerCharacter* InteractCharacter)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(InteractCharacter);
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "PlayerCharacter is null!, function: ALiveTrigger::Interact_Implementation()")));
		return;
	}
	if (PlayerCharacter->HasItem(RequireItemType))
	{
		IInteractableInterface::Interact_Implementation(InteractCharacter);
		PlayerCharacter->RemoveInventoryItem(RequireItemType);
		OnInteract.Broadcast();
		IsActivated = true;
	}
	else
	{
		SubtitleActor->ShowSubtitleFromScript(SubtitleText.ToString(), SubtitleDuration);
	}
}

void ALiveTrigger::ToggleOutline_Implementation(bool bValue)
{
	IInteractableInterface::ToggleOutline_Implementation(bValue);
	InteractionHUD->SetVisibility(bValue);
}

bool ALiveTrigger::IsEnable_Implementation()
{
	return !IsActivated;
}

void ALiveTrigger::StartCheckAndUpdateWidgetVisibleTimer_Implementation()
{
	IInteractableInterface::StartCheckAndUpdateWidgetVisibleTimer_Implementation();

	GetWorldTimerManager().SetTimer(
		CheckAndUpdateWidgetVisibleTimer, this, &ALiveTrigger::CheckAndUpdateWidgetVisible_Implementation, 0.1f, true);
}

void ALiveTrigger::CheckAndUpdateWidgetVisible_Implementation()
{
	IInteractableInterface::CheckAndUpdateWidgetVisible_Implementation();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerCharacter is null!")));
		return;
	}
	if (IsActivated || PlayerCharacter->FocusActor != this || !PlayerCharacter->CloseActors.Contains(this))
	{
		ToggleOutline_Implementation(false);
		PlayerCharacter->FocusActor = nullptr;
		GetWorldTimerManager().ClearTimer(CheckAndUpdateWidgetVisibleTimer);
	}
}

bool ALiveTrigger::IsInteractiveHUDVisible_Implementation()
{
	return InteractionHUD->IsVisible();
}
