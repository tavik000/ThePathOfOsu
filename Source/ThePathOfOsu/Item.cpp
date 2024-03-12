// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

bool UItem::IsConsumable() const
{
	return MaxCount > 0;
}

FString UItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId UItem::GetPrimaryAssetId() const
{
	return Super::GetPrimaryAssetId();
}

