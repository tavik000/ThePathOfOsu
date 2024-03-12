// Fill out your copyright notice in the Description page of Project Settings.


#include "UseItemNotify.h"

#include "OxCharacter.h"
#include "PlayerCharacter.h"

void UUseItemNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		Character->UseSlotItem();
	}
}
