#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "WeaponBase.h"
#include "GunBase.generated.h"

UCLASS()
class THEPATHOFOSU_API AGunBase : public AWeaponBase
{
	GENERATED_BODY()

public:
	AGunBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Shoot();
	
private:
	
	UPROPERTY(EditAnywhere)
	float MaxRange = 5000.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;

	bool TryGunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* OwnerController;
	
	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;
	
	UPROPERTY(EditAnywhere)
	USoundAttenuation* AttenuationSettings;
	
	UPROPERTY(EditAnywhere)
	USoundConcurrency* SoundConcurrencySettings;
	
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* ImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* FireMontage;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BulletTraceActorClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ImpactDecalActorClass;

	UPROPERTY(EditAnywhere)
	FName MuzzleSocketName = TEXT("MuzzleFlashSocket");
};
