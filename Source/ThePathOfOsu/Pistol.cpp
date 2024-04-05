

#include "Pistol.h"


APistol::APistol()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
	
}

void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

