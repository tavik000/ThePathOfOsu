#pragma once

#include "UObject/PrimaryAssetId.h"
#include "OsuType.generated.h"

USTRUCT(BlueprintType)
struct THEPATHOFOSU_API FOsuType
{
	GENERATED_BODY()
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

UENUM(BlueprintType)
enum class EAnimationState : uint8 {
	Unarmed = 0 UMETA(DisplayName = "Unarmed"),
	Pistol = 1  UMETA(DisplayName = "Pistol"),
	Rifle = 2 UMETA(DisplayName = "Rifle"),
};

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	UnEquip = 0 UMETA(DisplayName = "UnEquip"),
	Equip = 1  UMETA(DisplayName = "Equip"),
};
