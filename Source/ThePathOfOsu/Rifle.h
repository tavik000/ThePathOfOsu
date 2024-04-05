
#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Rifle.generated.h"

UCLASS()
class THEPATHOFOSU_API ARifle : public AWeaponBase
{
	GENERATED_BODY()

public:
	ARifle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
