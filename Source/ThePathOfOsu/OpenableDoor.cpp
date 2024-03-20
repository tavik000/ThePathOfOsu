// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenableDoor.h"

#include "PlayerCharacter.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"


AOpenableDoor::AOpenableDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	IsActivated = false;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	OpenAngle = 90.0f;
	DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMesh->SetupAttachment(RootComp);
	InteractionHUD = CreateDefaultSubobject<UBillboardComponent>(TEXT("InteractionHUD"));
	InteractionHUD->SetupAttachment(RootComp);
	InteractionHUD->SetRelativeLocation(FVector(0.0f, -48.0f, 31.0f));
	InteractionHUD->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	InteractionHUD->SetHiddenInGame(false);
}

void AOpenableDoor::BeginPlay()
{
	Super::BeginPlay();
	InteractionHUD->SetVisibility(false);
}

void AOpenableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOpenableDoor::Interact_Implementation()
{
	IInteractableInterface::Interact_Implementation();
	IsActivated = true;
	OnInteracted.Broadcast();
}

bool AOpenableDoor::IsEnable_Implementation()
{
	return !IsActivated;
}

void AOpenableDoor::ToggleOutline_Implementation(bool bValue)
{
	IInteractableInterface::ToggleOutline_Implementation(bValue);
	InteractionHUD->SetVisibility(bValue);
}

void AOpenableDoor::StartCheckAndUpdateWidgetVisibleTimer_Implementation()
{
	IInteractableInterface::StartCheckAndUpdateWidgetVisibleTimer_Implementation();
	GetWorldTimerManager().SetTimer(
		CheckAndUpdateWidgetVisibleTimer, this, &AOpenableDoor::CheckAndUpdateWidgetVisible_Implementation, 0.1f, true);
}

void AOpenableDoor::CheckAndUpdateWidgetVisible_Implementation()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerCharacter is null!")));
		return;
	}
	if (IsActivated || PlayerCharacter->FocusActor != this || !PlayerCharacter->CloseActors.Contains(this))
	{
		ToggleOutline_Implementation(false);
		GetWorldTimerManager().ClearTimer(CheckAndUpdateWidgetVisibleTimer);
	}
}

bool AOpenableDoor::IsInteractiveHUDVisible_Implementation()
{
	return InteractionHUD->IsVisible();
}
