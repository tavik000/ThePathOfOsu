// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEPATHOFOSU_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTransporter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnButtonActivated();
	
	UFUNCTION()
	void OnBackwardButtonActivated();

	UFUNCTION()
	void OnButtonDeactivated();

	void Reset();

	FVector StartPoint;
	FVector EndPoint;
	bool ArePointsSet;
	UPROPERTY(EditAnywhere)
	float MoveTime;
	UPROPERTY(EditAnywhere)
	bool IsOwnerTriggerActor;
	UPROPERTY(EditAnywhere)
	AActor* TriggerActor;
	UPROPERTY(EditAnywhere)
	AActor* BackwardTriggerActor;
	UPROPERTY(VisibleAnywhere)
	bool IsTriggered;
	
	bool IsGoingBackward = false;

	UFUNCTION(BlueprintCallable)
	void SetPoints(FVector ToSetStartPoint, FVector ToSetEndPoint);

private:
	float Speed;
		
};
