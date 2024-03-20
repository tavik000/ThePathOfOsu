
#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "OsuType.h"
#include "Subtitle.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "LiveTrigger.generated.h"

UCLASS()
class THEPATHOFOSU_API ALiveTrigger : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ALiveTrigger();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(AActor* InteractActor) override;
	virtual void ToggleOutline_Implementation(bool bValue) override;
	virtual bool IsEnable_Implementation() override;
	virtual void StartCheckAndUpdateWidgetVisibleTimer_Implementation() override;
	virtual void CheckAndUpdateWidgetVisible_Implementation() override;
	virtual bool IsInteractiveHUDVisible_Implementation() override;

	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsActivated;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBillboardComponent* InteractionHUD;


	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnInteract OnInteract;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UItem *RequireItemType;

	UPROPERTY(EditDefaultsOnly)
	ASubtitle* SubtitleActor;

	UPROPERTY(EditDefaultsOnly)
	FText SubtitleText;

	UPROPERTY(EditDefaultsOnly)
	float SubtitleDuration;
	
private:
	FTimerHandle CheckAndUpdateWidgetVisibleTimer;
	
};
