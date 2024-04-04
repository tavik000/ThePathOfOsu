
#pragma once

#include "CoreMinimal.h"
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
};
