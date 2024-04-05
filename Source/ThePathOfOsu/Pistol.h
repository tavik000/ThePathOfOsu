// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunBase.h"
#include "WeaponBase.h"
#include "Pistol.generated.h"

UCLASS()
class THEPATHOFOSU_API APistol : public AGunBase
{
	GENERATED_BODY()

public:
	APistol();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
