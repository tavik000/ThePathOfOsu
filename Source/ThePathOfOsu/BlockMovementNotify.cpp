// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockMovementNotify.h"

#include "OxCharacter.h"

void UBlockMovementNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->BlockMovementReasons.Add(StaticClass()->GetFName().ToString());
	}
}

void UBlockMovementNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->BlockMovementReasons.Remove(StaticClass()->GetFName().ToString());
	}
}
