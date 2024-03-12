#include "CollectableActor.h"
#include "PlayerCharacter.h"


ACollectableActor::ACollectableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComp);
	Capsule->SetCollisionProfileName(FName("OverlapAllDynamic"));
	Capsule->SetCapsuleHalfHeight(150.f);
	Capsule->SetCapsuleRadius(100.f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	CollectAudio->SetupAttachment(RootComp);
	CollectAudio->SetAutoActivate(false);
	RotationSpeed = 100.0f;
}

void ACollectableActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACollectableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
	TArray<AActor*> OverlappingActors;
	Capsule->GetOverlappingActors(OverlappingActors, APlayerCharacter::StaticClass());
	if (!OverlappingActors.IsEmpty())
	{
		if (!IsCollected)
		{
			Collect();
		}
	}
}

void ACollectableActor::Collect()
{
	IsCollected = true;
	OnCollected.Broadcast();
	CollectAudio->Play();
	Mesh->SetVisibility(false);
}
