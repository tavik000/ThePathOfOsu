// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockAnimNotify.h"

#include "OxCharacter.h"

void UBlockAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->IsBlocking = true;
	}
}

void UBlockAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->IsBlocking = false;
	}
}
