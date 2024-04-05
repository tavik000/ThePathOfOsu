

#include "Rifle.h"


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

