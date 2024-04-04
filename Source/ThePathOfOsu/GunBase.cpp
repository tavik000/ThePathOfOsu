

#include "GunBase.h"


AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

