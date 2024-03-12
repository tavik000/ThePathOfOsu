// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableActor.h"

#include "PlayerCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


APushableActor::APushableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	IsBeingPushed = false;
}

void APushableActor::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &APushableActor::OnHit);
	SetupTimeline();
	BoxSize = Mesh->GetRelativeLocation().Z * 2.0f;
	TravelDistance = BoxSize / 2.0f;
}

void APushableActor::SetupTimeline()
{
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));
		CurveTimeline.AddInterpFloat(CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(false);
		TimelineFinishedEvent.BindUFunction(this, FName("TimelineFinished"));
		CurveTimeline.SetTimelineFinishedFunc(TimelineFinishedEvent);
	}
}

void APushableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);
}

void APushableActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	FVector PushingActorForwardVector = OtherActor->GetActorForwardVector();
	if (IsPushingDiagonal(PushingActorForwardVector, Hit.Normal))
	{
		return;
	}
	PushingPlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!PushingPlayerCharacter || !CanPush(PushingPlayerCharacter))
	{
		return;
	}
	UPawnMovementComponent* MovementComponent = PushingPlayerCharacter->GetMovementComponent();
	if (!MovementComponent)
	{
		return;
	}
	MovementComponent->Deactivate();
	if (IsBeingPushed && PushingPlayerCharacter->IsPushing())
	{
		Push();
	}
	else
	{
		if (PushingPlayerCharacter->TryPush())
		{
			PushingPlayerCharacter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			PushingPlayerCharacter->OnBeginPush.AddDynamic(this, &APushableActor::Push);
		}
	}
}

bool APushableActor::IsPushingDiagonal(FVector PushingActorForwardVector, FVector HitNormal,
                                       double StraightDirectionTolerance)
{
	double HitNormalAndForwardDot = UKismetMathLibrary::Dot_VectorVector(HitNormal, PushingActorForwardVector);

	PushingDirection = FVector(UKismetMathLibrary::Round(PushingActorForwardVector.X),
	                           UKismetMathLibrary::Round(PushingActorForwardVector.Y), 0.0f);

	bool IsPushingDiagonal = UKismetMathLibrary::Abs(PushingDirection.X) + UKismetMathLibrary::Abs(PushingDirection.Y) >
		1.0f
		|| HitNormalAndForwardDot < StraightDirectionTolerance;
	return IsPushingDiagonal;
}

bool APushableActor::CanPush(AActor* PushingActor)
{
	FVector PushingActorLocation = PushingActor->GetActorLocation();
	FVector PushingActorForwardVector = PushingActor->GetActorForwardVector();
	FVector PushingActorTraceEndLocation = PushingActorLocation + PushingActorForwardVector * 100.0f;
	FHitResult PushingActorTraceHitResult;
	FCollisionQueryParams PushingActorTraceCollisionParams;
	PushingActorTraceCollisionParams.AddIgnoredActor(PushingActor);
	GetWorld()->LineTraceSingleByChannel(PushingActorTraceHitResult, PushingActorLocation, PushingActorTraceEndLocation,
	                                     ECC_Visibility,
	                                     PushingActorTraceCollisionParams);
	bool IsPushingActorFacingToThisActor = PushingActorTraceHitResult.bBlockingHit &&
		PushingActorTraceHitResult.GetActor() == this;

	FHitResult SelfTraceHitResult;
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(PushingPlayerCharacter);
	FBoxSphereBounds BoxSphereBounds3d = Mesh->GetStaticMesh()->GetBounds();
	FVector BoxExtent = BoxSphereBounds3d.BoxExtent;
	FVector HalfSize = Mesh->GetRelativeScale3D() * BoxExtent * 0.9f;
	FVector ZOffset = FVector(0.0f, 0.0f, BoxSize / 2);
	FVector SelfTraceStartLocation = GetActorLocation() + ZOffset;
	FVector SelfTraceEndLocation = GetActorLocation() + PushingDirection * TravelDistance + ZOffset;
	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), SelfTraceStartLocation, SelfTraceEndLocation, HalfSize,
	                                     FRotator::ZeroRotator,
	                                     UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors,
	                                     EDrawDebugTrace::ForDuration, SelfTraceHitResult, true, FLinearColor::Red,
	                                     FLinearColor::Green, 5.0f);
	bool IsPathClear = !SelfTraceHitResult.bBlockingHit;

	FCollisionQueryParams ForwardDownTraceCollisionParams;
	PushingActorTraceCollisionParams.AddIgnoredActor(this);
	FHitResult ForwardDownTraceHitResult;
	FVector ForwardDownTraceStartLocation = GetActorLocation() + PushingDirection * (BoxSize / 2.0f) * 1.5f + ZOffset;
	FVector ForwardDownTraceEndLocation = ForwardDownTraceStartLocation + FVector(0.0f, 0.0f, -BoxSize);
	GetWorld()->LineTraceSingleByChannel(ForwardDownTraceHitResult, ForwardDownTraceStartLocation,
	                                     ForwardDownTraceEndLocation, ECC_Visibility, ForwardDownTraceCollisionParams);
	bool HasGroundForward = ForwardDownTraceHitResult.bBlockingHit;


	FCollisionQueryParams UpTraceCollisionParams;
	UpTraceCollisionParams.AddIgnoredActor(this);
	FHitResult UpTraceHitResult;
	FVector UpTraceStartLocation = GetActorLocation() + FVector(0.0f, 0.0f, BoxSize);;
	FVector UpTraceEndLocation = GetActorLocation() + FVector(0.0f, 0.0f, BoxSize * 2.0f);
	GetWorld()->LineTraceSingleByChannel(UpTraceHitResult, UpTraceStartLocation, UpTraceEndLocation, ECC_Visibility,
	                                     UpTraceCollisionParams);
	bool IsAboveClear = !UpTraceHitResult.bBlockingHit;

	UPawnMovementComponent* MovementComponent = PushingPlayerCharacter->GetMovementComponent();
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("MovementComponent is null!"));
		return false;
	}

	bool IsPushingActorFalling = MovementComponent->IsFalling();


	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White,
	//                                  FString::Printf(
	// 	                                 TEXT("IsPushingActorFacingToThisActor: %d"), IsPushingActorFacingToThisActor));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("IsPathClear: %d"), IsPathClear));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White,
	//                                  FString::Printf(TEXT("HasGroundForward: %d"), HasGroundForward));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("IsAboveClear: %d"), IsAboveClear));
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White,
	//                                  FString::Printf(TEXT("IsPushingActorFalling: %d"), IsPushingActorFalling));


	return IsPushingActorFacingToThisActor && IsPathClear && HasGroundForward && IsAboveClear && !IsPushingActorFalling;
}

void APushableActor::Push()
{
	IsBeingPushed = true;
	PushingStartLocation = GetActorLocation();
	CurveTimeline.PlayFromStart();
}

void APushableActor::StopPushing()
{
	if (!PushingPlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PushingPlayerCharacter is null!, %s"), *GetName());
		return;
	}

	IsBeingPushed = false;
	PushingPlayerCharacter->OnBeginPush.RemoveDynamic(this, &APushableActor::Push);
	PushingPlayerCharacter->EndPush();
	UPawnMovementComponent* MovementComponent = PushingPlayerCharacter->GetMovementComponent();
	if (MovementComponent)
	{
		MovementComponent->Activate();
		PushingPlayerCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}


void APushableActor::TimelineProgress(float Value)
{
	FVector TargetLocation = PushingStartLocation + PushingDirection * TravelDistance * Value;
	SetActorRelativeLocation(TargetLocation);
}

void APushableActor::TimelineFinished()
{
	if (!PushingPlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("TimelineFinished PushingPlayerCharacter is null!, %s"), *GetName());
		return;
	}
	if (PushingPlayerCharacter->IsMoveInputBeingPressed() && CanPush(PushingPlayerCharacter))
	{
		Push();
	}
	else
	{
		StopPushing();
	}
}
