#include "PressableButton.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

APressableButton::APressableButton()
{
	PrimaryActorTick.bCanEverTick = true;
	IsActivated = false;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	MeshOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshOutline"));
	MeshOutline->SetupAttachment(Mesh);
	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
	Transporter->MoveTime = 0.1f;
	Transporter->IsOwnerTriggerActor = true;
	InteractionHUD = CreateDefaultSubobject<UBillboardComponent>(TEXT("InteractionHUD"));
	InteractionHUD->SetupAttachment(RootComp);
	InteractionHUD->SetRelativeLocation(FVector(0.0f, -48.0f, 31.0f));
	InteractionHUD->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	InteractionHUD->SetHiddenInGame(false);
}

void APressableButton::SetupOutline_Implementation()
{
	IInteractableInterface::SetupOutline_Implementation();
	MeshOutline->SetStaticMesh(Mesh->GetStaticMesh());
	MeshOutline->SetRelativeScale3D(FVector(1.02f, 1.02f, 1.02f));


	if (!OutlineMaterial)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT(
			                                 "OutlineMaterial is null, function: AAPickup::SetupOutline_Implementation()")));
		UE_LOG(LogTemp, Error, TEXT("OutlineMaterial is null, function: AAPickup::SetupOutline_Implementation()"));
	}

	for (int i = 0; i <= Mesh->GetNumMaterials() - 1; i++)
	{
		// UMaterialInterface* OutlineMaterial = Cast<UMaterialInterface>(
		// 	StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/Materials/M_MeshOutline")));
		MeshOutline->SetMaterial(i, OutlineMaterial);
	}
	MeshOutline->SetVisibility(false);
	InteractionHUD->SetVisibility(false);
}

bool APressableButton::IsInteractiveHUDVisible_Implementation()
{
	return InteractionHUD->IsVisible();
}

void APressableButton::BeginPlay()
{
	Super::BeginPlay();
	Transporter->SetPoints(GetActorLocation(), GetActorLocation() + -GetActorForwardVector() * 5.0f);
	SetupOutline_Implementation();

	// FTimerHandle UnusedHandle;
	// FTimerDelegate TimerDelegate;
	// TimerDelegate.BindUFunction(this, FName("ToggleOutline"), true);
	// GetWorldTimerManager().SetTimer(
	// 	UnusedHandle, TimerDelegate, 5.0f, false);
}


void APressableButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APressableButton::Reset()
{
	Super::Reset();
	IsActivated = false;
	Transporter->Reset();
}


void APressableButton::Interact_Implementation(APlayerCharacter* InteractCharacter)
{
	IInteractableInterface::Interact_Implementation(InteractCharacter);
	OnActivated.Broadcast();
	IsActivated = true;
	if (IsToggleable)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			Reset();
		}, 3.0f, false);
	}

	if (PressSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PressSound, GetActorLocation());
	}
}

void APressableButton::ToggleOutline_Implementation(bool bValue)
{
	IInteractableInterface::ToggleOutline_Implementation(bValue);
	MeshOutline->SetVisibility(bValue);
	InteractionHUD->SetVisibility(bValue);
}

bool APressableButton::IsEnable_Implementation()
{
	return !IsActivated;
}

void APressableButton::StartCheckAndUpdateWidgetVisibleTimer_Implementation()
{
	IInteractableInterface::StartCheckAndUpdateWidgetVisibleTimer_Implementation();
	GetWorldTimerManager().SetTimer(
		CheckAndUpdateWidgetVisibleTimer, this, &APressableButton::CheckAndUpdateWidgetVisible_Implementation, 0.1f,
		true);
}

void APressableButton::CheckAndUpdateWidgetVisible_Implementation()
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
		PlayerCharacter->FocusActor = nullptr;
		GetWorldTimerManager().ClearTimer(CheckAndUpdateWidgetVisibleTimer);
	}
}
