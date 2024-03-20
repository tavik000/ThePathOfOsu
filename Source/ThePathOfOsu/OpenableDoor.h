#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "OpenableDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpened);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorInteracted);

UCLASS()
class THEPATHOFOSU_API AOpenableDoor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AOpenableDoor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APlayerCharacter* InteractCharacter) override;
	virtual bool IsEnable_Implementation() override;
	virtual void ToggleOutline_Implementation(bool bValue) override;
	virtual void StartCheckAndUpdateWidgetVisibleTimer_Implementation() override;
	virtual void CheckAndUpdateWidgetVisible_Implementation() override;
	virtual bool IsInteractiveHUDVisible_Implementation() override;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* DoorFrameMesh;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBillboardComponent* InteractionHUD;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnDoorOpened OnOpen;

	UPROPERTY(BlueprintAssignable)
	FOnDoorInteracted OnInteracted;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OpenAngle;

private:
	FTimerHandle CheckAndUpdateWidgetVisibleTimer;
	bool IsActivated;
};
