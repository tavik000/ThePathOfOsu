// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystemComponent.h"
#include "OxCharacter.h"
#include "PlayerCharacter.h"


UWeaponSystemComponent::UWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AOxCharacter>(GetOwner());
	Rifle = Cast<ARifle>(OwnerCharacter->RifleChildActorComponent->GetChildActor());
	Pistol = Cast<APistol>(OwnerCharacter->PistolChildActorComponent->GetChildActor());
	if (!Rifle || !Pistol)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Rifle or Pistol is null")));
		return;
	}
	Rifle->SetOwner(OwnerCharacter);
	Pistol->SetOwner(OwnerCharacter);
	GetWorld()->GetTimerManager().SetTimer(RifleFireTimerHandle, this, &UWeaponSystemComponent::CheckRifleFire,
	                                       RifleFireRate, true);
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
	if (PlayerCharacter)
	{
		PlayerCharacter->OnPlayerAddItem.AddDynamic(this, &UWeaponSystemComponent::OnPlayerAddItem);
	}
	
}


void UWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponSystemComponent::TryFire()
{
	switch (OwnerCharacter->GetCurrentAnimationState())
	{
	case EAnimationState::Rifle:
		IsRifleFiring = true;
		break;
	case EAnimationState::Pistol:
		Pistol->Shoot();
		PlayFireMontage();
		break;
	default: ;
	}
}

void UWeaponSystemComponent::OnFireActionEnd()
{
	switch (OwnerCharacter->GetCurrentAnimationState())
	{
	case EAnimationState::Rifle:
		IsRifleFiring = false;
		break;
	case EAnimationState::Pistol:
		break;
	default: ;
	}
}

void UWeaponSystemComponent::PlayFireMontage()
{
	switch (OwnerCharacter->GetCurrentAnimationState())
	{
	case EAnimationState::Rifle:
		OwnerCharacter->PlayMontage(RifleFireMontage);
		break;
	case EAnimationState::Pistol:
		OwnerCharacter->PlayMontage(PistolFireMontage);
		break;
	default: ;
	}
}

void UWeaponSystemComponent::UnequipAllWeapon()
{
	switch (OwnerCharacter->GetCurrentAnimationState())
	{
	case EAnimationState::Rifle:
		UnequipRifle();
		break;
	case EAnimationState::Pistol:
		UnequipPistol();
		break;
	default: ;
	}
}

void UWeaponSystemComponent::EquipRifle()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		if (PlayerCharacter->GetIsTargetLocking())
		{
			PlayerCharacter->UnlockTarget();
		}
	}
	if (OwnerCharacter->GetCurrentAnimationState() == EAnimationState::Rifle) return;
	if (OwnerCharacter->GetCurrentAnimationState() == EAnimationState::Pistol)
	{
		UnequipPistol();
	}

	OwnerCharacter->RifleChildActorComponent->AttachToComponent(OwnerCharacter->GetMesh(),
	                                                            FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                                            RifleHandSocketName);
	OwnerCharacter->SetAnimationState(EAnimationState::Rifle);
}

void UWeaponSystemComponent::UnequipRifle()
{
	OwnerCharacter->RifleChildActorComponent->AttachToComponent(OwnerCharacter->RifleSceneComponent,
	                                                            FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                                            RifleHostSocketName);
	OwnerCharacter->SetAnimationState(EAnimationState::Unarmed);
}

void UWeaponSystemComponent::EquipPistol()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter))
	{
		if (PlayerCharacter->GetIsTargetLocking())
		{
			PlayerCharacter->UnlockTarget();
		}
	}
	if (OwnerCharacter->GetCurrentAnimationState() == EAnimationState::Pistol) return;
	if (OwnerCharacter->GetCurrentAnimationState() == EAnimationState::Rifle)
	{
		UnequipRifle();
	}

	OwnerCharacter->PistolChildActorComponent->AttachToComponent(OwnerCharacter->GetMesh(),
	                                                             FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                                             PistolHandSocketName);
	OwnerCharacter->SetAnimationState(EAnimationState::Pistol);
}

void UWeaponSystemComponent::UnequipPistol()
{
	OwnerCharacter->PistolChildActorComponent->AttachToComponent(OwnerCharacter->PistolSceneComponent,
	                                                             FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                                             PistolHostSocketName);
	OwnerCharacter->SetAnimationState(EAnimationState::Unarmed);
}

void UWeaponSystemComponent::CheckRifleFire()
{
	if (IsRifleFiring && OwnerCharacter->GetCurrentAnimationState() == EAnimationState::Rifle)
	{
		Rifle->Shoot();
		PlayFireMontage();
	}
}

void UWeaponSystemComponent::StartSprint()
{
	if (OwnerCharacter->GetCurrentAnimationState() == EAnimationState::Rifle)
	{
		OwnerCharacter->RifleChildActorComponent->AttachToComponent(OwnerCharacter->GetMesh(),
		                                                            FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                                            RifleJogSocketName);
	}
}

void UWeaponSystemComponent::EndSprint()
{
	if (OwnerCharacter->GetCurrentAnimationState() == EAnimationState::Rifle)
	{
		OwnerCharacter->RifleChildActorComponent->AttachToComponent(OwnerCharacter->GetMesh(),
		                                                             FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                                             RifleHandSocketName);
	}
}

void UWeaponSystemComponent::OnPlayerAddItem(UItem* Item)
{
	if (Item->ItemName.EqualTo(FText::FromString("Rifle")))
	{
		EquipRifle();
		Rifle->PlayPickUpSound();
	}
	
	if (Item->ItemName.EqualTo(FText::FromString("Pistol")))
	{
		EquipPistol();
	}
}
