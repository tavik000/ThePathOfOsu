// Copyright Key. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item.generated.h"

UCLASS()
class THEPATHOFOSU_API UItem : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UItem()
		: MaxCount(1)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxCount;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsConsumable() const;


	UFUNCTION(BlueprintCallable, Category = "Item")
	FString GetIdentifierString() const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
