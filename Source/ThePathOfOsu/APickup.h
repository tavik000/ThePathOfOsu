
#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "OsuType.h"
#include "APickup.generated.h"

UCLASS()
class THEPATHOFOSU_API AAPickup : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AAPickup();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	bool GiveItem();

	virtual void Interact_Implementation(AActor* InteractActor) override;
	virtual void ToggleOutline_Implementation(bool bValue) override;
	virtual bool IsEnable_Implementation() override;
	virtual void StartCheckAndUpdateWidgetVisibleTimer_Implementation() override;
	virtual void CheckAndUpdateWidgetVisible_Implementation() override;
	virtual void SetupOutline_Implementation() override;
	virtual bool IsInteractiveHUDVisible_Implementation() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* MeshOutline;

	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBillboardComponent* InteractionHUD;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTextRenderComponent* ObjectNameText;

	UPROPERTY(EditAnywhere)
	UItem* ItemType;

	UPROPERTY(EditAnywhere)
	USoundBase* GiveItemSound;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* OutlineMaterial;
	
private:
	FTimerHandle CheckAndUpdateWidgetVisibleTimer;
};
