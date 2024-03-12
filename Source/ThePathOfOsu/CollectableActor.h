// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "CollectableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectableActorCollected);

UCLASS()
class THEPATHOFOSU_API ACollectableActor : public AActor
{
	GENERATED_BODY()

public:
	ACollectableActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* Capsule;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsCollected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed;


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UAudioComponent* CollectAudio;

	FOnCollectableActorCollected OnCollected;

	UFUNCTION(BlueprintCallable)
	void Collect();
};
