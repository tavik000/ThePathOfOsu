

#include "Subtitle.h"


ASubtitle::ASubtitle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASubtitle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASubtitle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


