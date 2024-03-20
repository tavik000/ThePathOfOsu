#include "APickup.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AAPickup::AAPickup()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	MeshOutline = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshOutline"));
	MeshOutline->SetupAttachment(Mesh);
	InteractionHUD = CreateDefaultSubobject<UBillboardComponent>(TEXT("InteractionHUD"));
	InteractionHUD->SetupAttachment(RootComp);
	InteractionHUD->SetRelativeLocation(FVector(0.0f, -48.0f, 31.0f));
	InteractionHUD->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	InteractionHUD->SetHiddenInGame(false);
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComp);
	FColor ArrowColor = FColor::FromHex(TEXT("7F7CFFFF"));
	Arrow->SetArrowColor(ArrowColor);
	ObjectNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ObjectNameText"));
	ObjectNameText->SetupAttachment(InteractionHUD);
	ObjectNameText->SetRelativeLocation(FVector(0.0f, 96.0f, 125.0f));
}

void AAPickup::BeginPlay()
{
	Super::BeginPlay();
	SetupOutline_Implementation();
}

void AAPickup::SetupOutline_Implementation()
{
	IInteractableInterface::SetupOutline_Implementation();
	MeshOutline->SetStaticMesh(Mesh->GetStaticMesh());
	MeshOutline->SetRelativeScale3D(FVector(1.02f, 1.02f, 1.02f));

	if (!OutlineMaterial)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("OutlineMaterial is null, function: AAPickup::SetupOutline_Implementation()")));
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
	ObjectNameText->SetVisibility(false);
}

bool AAPickup::IsInteractiveHUDVisible_Implementation()
{
	return InteractionHUD->IsVisible();
}

void AAPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ObjectNameText->IsVisible())
	{
		FVector CameraLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->
			GetComponentLocation();
		FRotator ToSetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CameraLocation);
		ObjectNameText->SetWorldRotation(ToSetRotation);
	}
}

bool AAPickup::GiveItem()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerCharacter is null!")));
		return false;
	}
	const bool IsGiveItemSuccessful = PlayerCharacter->AddInventoryItem(ItemType, 1);
	if (IsGiveItemSuccessful)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GiveItemSound, GetActorLocation());
		Destroy();
	}
	return IsGiveItemSuccessful;
}

void AAPickup::Interact_Implementation()
{
	IInteractableInterface::Interact_Implementation();
	OnInteract.Broadcast();
	GiveItem();
}

void AAPickup::ToggleOutline_Implementation(bool bValue)
{
	IInteractableInterface::ToggleOutline_Implementation(bValue);
	MeshOutline->SetVisibility(bValue);
	InteractionHUD->SetVisibility(bValue);
	ObjectNameText->SetVisibility(bValue);
}

bool AAPickup::IsEnable_Implementation()
{
	return true;
}

void AAPickup::StartCheckAndUpdateWidgetVisibleTimer_Implementation()
{
	IInteractableInterface::StartCheckAndUpdateWidgetVisibleTimer_Implementation();
	GetWorldTimerManager().SetTimer(
		CheckAndUpdateWidgetVisibleTimer, this, &AAPickup::CheckAndUpdateWidgetVisible_Implementation, 0.1f, true);
}

void AAPickup::CheckAndUpdateWidgetVisible_Implementation()
{
	IInteractableInterface::CheckAndUpdateWidgetVisible_Implementation();

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!PlayerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PlayerCharacter is null!")));
		return;
	}
	if (PlayerCharacter->FocusActor != this || !PlayerCharacter->CloseActors.Contains(this))
	{
		ToggleOutline_Implementation(false);
		PlayerCharacter->FocusActor = nullptr;
		GetWorldTimerManager().ClearTimer(CheckAndUpdateWidgetVisibleTimer);
	}
}
