// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LomInventoryItemDefinition.h"
#include "InventoryFragment_EquippableItem.generated.h"

/**
 * 
 */
class ULomEquipmentItemDefinition;

UCLASS()
class LOMCHARACTERSYSTEM_API UInventoryFragment_EquippableItem : public ULomInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText name;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ULomEquipmentItemDefinition> EquipmentDefinition;
};
