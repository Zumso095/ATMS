// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LomInventoryItemDefinition.h"

const ULomInventoryItemFragment* ULomInventoryItemDefinition::FindFragment(TSubclassOf<ULomInventoryItemFragment> fragmentClass) const
{
	//GEngine->AddOnScreenDebugMessage(-1, 33.f, FColor::Green, "find fragment called");

	if (fragmentClass != nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 33.f, FColor::Green, "find fragment class correct");

		for (ULomInventoryItemFragment* fragmentItem : Fragments)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 33.f, FColor::Green, fragmentItem->GetName());
			if (fragmentItem->IsA(fragmentClass))
			{
				return fragmentItem;
			}
	}
	
	}

	return nullptr;
}

void ULomInventoryItemFragment::OnInstanceCreatedEvent_Implementation(ULomInventoryItemInstance* ItemInstance)
{
}
