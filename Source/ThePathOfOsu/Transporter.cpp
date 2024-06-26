#include "Transporter.h"

#include "PressableButton.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	MoveTime = 3.0f;
	ArePointsSet = false;
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
}


void UTransporter::BeginPlay()
{
	Super::BeginPlay();
	if (IsOwnerTriggerActor)
	{
		ForwardTriggerActor = GetOwner();
	}

	if (!ForwardTriggerActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("TriggerActor is null! %s"), *GetOwner()->GetName()));
		return;
	}
	if (APressableButton* PressableButton = Cast<APressableButton>(ForwardTriggerActor))
	{
		PressableButton->OnActivated.AddDynamic(this, &UTransporter::OnButtonActivated);
		PressableButton->OnDeactivated.AddDynamic(this, &UTransporter::OnButtonDeactivated);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("TriggerActor is not a PressableButton!")));
		return;
	}

	if (BackwardTriggerActor)
	{
		if (APressableButton* PressableButton = Cast<APressableButton>(BackwardTriggerActor))
		{
			PressableButton->OnActivated.AddDynamic(this, &UTransporter::OnBackwardButtonActivated);
			PressableButton->OnDeactivated.AddDynamic(this, &UTransporter::OnButtonDeactivated);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			                                 FString::Printf(TEXT("TriggerActor is not a PressableButton!")));
			return;
		}
	}
}


void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* MyOwner = GetOwner();
	if (MyOwner && ArePointsSet)
	{
		FVector CurrentLocation = MyOwner->GetActorLocation();
		FVector TargetLocation;
		if (IsGoingBackward)
		{
			TargetLocation = IsTriggered ? StartPoint : EndPoint;
		}
		else
		{
			TargetLocation = IsTriggered ? EndPoint : StartPoint;
		}
		if (IsTriggered && !CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			MyOwner->SetActorLocation(NewLocation);
		}
	}
}

void UTransporter::OnButtonActivated()
{
	IsGoingBackward = false;
	IsTriggered = true;
}

void UTransporter::OnBackwardButtonActivated()
{
	IsGoingBackward = true;
	IsTriggered = true;
}

void UTransporter::OnButtonDeactivated()
{
	IsTriggered = false;
}

void UTransporter::Reset()
{
	IsTriggered = false;
	GetOwner()->SetActorLocation(StartPoint);
}

void UTransporter::SetPoints(FVector ToSetStartPoint, FVector ToSetEndPoint)
{
	if (ToSetStartPoint.Equals(ToSetEndPoint))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(TEXT("Start and End points are the same!")));
		return;
	}
	StartPoint = ToSetStartPoint;
	EndPoint = ToSetEndPoint;
	ArePointsSet = true;
	Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;
}
