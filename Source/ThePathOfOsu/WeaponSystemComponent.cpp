// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystemComponent.h"
#include "OxCharacter.h"


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
		Rifle->Shoot();
		break;
	case EAnimationState::Pistol:
		Pistol->Shoot();
		break;
	default: ;
	}
}

void UWeaponSystemComponent::OnFireActionEnd()
{
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
