#include "GunBase.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"


AGunBase::AGunBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGunBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		const APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (OwnerPawn == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("OwnerPawn is null")));
			return;
		}
	
		OwnerController = OwnerPawn->GetController();
		if (OwnerController == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("OwnerController is null")));
		}
	});
}

void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGunBase::Shoot()
{
	// UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, GunMeshComponent, TEXT("MuzzleFlashSocket"));
	// UGameplayStatics::SpawnSoundAttached(MuzzleSound, GunMeshComponent, TEXT("MuzzleFlashSocket"));
	FHitResult Hit;
	FVector ShotDirection;
	if (bool IsHit = TryGunTrace(Hit, ShotDirection))
	{
		// DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		// UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
		// UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

bool AGunBase::TryGunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	if (OwnerController == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("OwnerController is null")));
		return false;
	}
	OwnerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	const FVector EndLocation = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * MaxRange;

	ShotDirection = -PlayerViewPointRotation.Vector();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	bool IsHit = GetWorld()->LineTraceSingleByChannel(Hit, PlayerViewPointLocation, EndLocation, ECC_GameTraceChannel1,
	                                                  Params);
	return IsHit;
}
