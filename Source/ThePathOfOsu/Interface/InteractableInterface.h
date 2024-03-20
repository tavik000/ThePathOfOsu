#pragma once

#include "CoreMinimal.h"
#include "ThePathOfOsu/PlayerCharacter.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class THEPATHOFOSU_API IInteractableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void Interact(APlayerCharacter* InteractPlayerCharacter);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	bool IsEnable();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void ToggleOutline(bool bValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	bool IsInteractiveHUDVisible();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void StartCheckAndUpdateWidgetVisibleTimer();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void CheckAndUpdateWidgetVisible();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
	void SetupOutline();
};
