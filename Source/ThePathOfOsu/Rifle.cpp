#include "Rifle.h"

#include "Kismet/GameplayStatics.h"


ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARifle::PlayPickUpSound()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
}
