// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackReflectableAnimNotify.h"

#include "OxCharacter.h"

void UAttackReflectableAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->IsAttackReflectable = true;
	}
}

void UAttackReflectableAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->IsAttackReflectable = false;
	}
}
