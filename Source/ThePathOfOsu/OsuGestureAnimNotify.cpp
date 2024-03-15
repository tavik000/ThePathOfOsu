// Fill out your copyright notice in the Description page of Project Settings.


#include "OsuGestureAnimNotify.h"

#include "OxCharacter.h"


void UOsuGestureAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	AOxCharacter* Character = Cast<AOxCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->OsuGestureRestorePosture();
	}
}
