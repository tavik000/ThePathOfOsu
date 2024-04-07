
#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Transporter.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "Interface/InteractableInterface.h"
#include "PressableButton.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressableButtonOnActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressableButtonOnDeactivated);

UCLASS()
class THEPATHOFOSU_API APressableButton : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	APressableButton();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void Interact_Implementation(APlayerCharacter* InteractCharacter) override;
	void ToggleOutline_Implementation(bool bValue) override;
	bool IsEnable_Implementation() override;
	void StartCheckAndUpdateWidgetVisibleTimer_Implementation() override;
	void CheckAndUpdateWidgetVisible_Implementation() override;
	void SetupOutline_Implementation() override;;
	bool IsInteractiveHUDVisible_Implementation() override;

	void Reset();
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* MeshOutline;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsActivated;

	UPROPERTY(EditAnywhere)
	bool IsToggleable = false;

	UPROPERTY(BlueprintAssignable)
	FPressableButtonOnActivated OnActivated;
	
	UPROPERTY(BlueprintAssignable)
	FPressableButtonOnDeactivated OnDeactivated;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTransporter* Transporter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBillboardComponent* InteractionHUD;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* OutlineMaterial;
	
private:
	FTimerHandle CheckAndUpdateWidgetVisibleTimer;
};
