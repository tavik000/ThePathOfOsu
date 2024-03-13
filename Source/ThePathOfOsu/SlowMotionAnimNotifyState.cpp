// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowMotionAnimNotifyState.h"

#include "OxCharacter.h"
#include "Kismet/GameplayStatics.h"

void USlowMotionAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetTimeScale(TimeScale);
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, FString::Printf(TEXT("Slow Motion")));
}

void USlowMotionAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->SetTimeScale(1.0f);
	}
}
