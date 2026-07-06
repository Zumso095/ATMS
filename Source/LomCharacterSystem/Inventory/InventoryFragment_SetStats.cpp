// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryFragment_SetStats.h"
#include "Inventory/LomInventoryItemInstance.h"

void UInventoryFragment_SetStats::OnInstanceCreated(ULomInventoryItemInstance* Instance)
{
	Super::OnInstanceCreated(Instance);
	for (const FGameplayTagStack& stack : TagStacks)
	{
		Instance->AddTagStack(stack.Tag, stack.Count);
	}
}
