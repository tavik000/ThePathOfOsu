// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockReflectableAnimNotify.h"

#include "OxCharacter.h"

void UBlockReflectableAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->IsBlockReflectable = true;
	}
}

void UBlockReflectableAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->IsBlockReflectable = false;
	}
}
