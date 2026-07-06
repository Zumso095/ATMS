// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LomInventoryItemDefinition.h"
#include "System/GameplayTagStack.h"
#include "InventoryFragment_SetStats.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API UInventoryFragment_SetStats : public ULomInventoryItemFragment
{
	GENERATED_BODY()
public:
	
	virtual void OnInstanceCreated(ULomInventoryItemInstance* Instance) override;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTagStack> TagStacks;
};
