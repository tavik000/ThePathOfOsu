#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Subtitle.generated.h"

UCLASS()
class THEPATHOFOSU_API ASubtitle : public AActor
{
	GENERATED_BODY()

public:
	ASubtitle();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowSubtitle(const FString& SubtitleText, float Duration);
};
