// Fill out your copyright notice in the Description page of Project Settings.


#include "FistAttackAnimNotify.h"

#include "OxCharacter.h"

void UFistAttackAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->BeginFistAttack(IsLeftFist);
	}
	
}

void UFistAttackAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->EndFistAttack(IsLeftFist);
	}
	
}
