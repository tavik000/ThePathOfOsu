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
	UNARMED = 0 UMETA(DisplayName = "Unarmed"),
	PISTOL = 1  UMETA(DisplayName = "Pistol"),
	RIFLE = 2 UMETA(DisplayName = "Rifle"),
};