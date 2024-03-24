// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PushableActor.generated.h"

class UCurveFloat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPushableActorOnPushFinished);

UCLASS()
class THEPATHOFOSU_API APushableActor : public AActor
{
	GENERATED_BODY()

public:
	APushableActor();
	void SetupTimeline();

protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	bool CanPush(AActor* PushingActor);

	UFUNCTION()
	void Push();

	UFUNCTION()
	void StopPushing(bool IsInterrupt);

	UFUNCTION()
	void InterruptPushing();

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacter* PushingPlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector PushingDirection;

	UPROPERTY(EditAnywhere)
	float TravelDistance;

	FVector PushingStartLocation;

	FOnTimelineEvent TimelineFinishedEvent;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void TimelineProgress(float Value);

	UFUNCTION()
	void TimelineFinished();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsBeingPushed;

	UPROPERTY(BlueprintAssignable)
	FPushableActorOnPushFinished OnPushFinished;

private:
	bool IsPushingDiagonal(FVector PushingActorForwardVector, FVector HitNormal, double StraightDirectionTolerance = 0.95f);

	float BoxSize;
	
	
};
