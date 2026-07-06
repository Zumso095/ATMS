// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LomInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

void ULomInventoryItemInstance::SetItemDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition)
{
	itemDefinition = ItemDefinition;
}

TSubclassOf<ULomInventoryItemDefinition> ULomInventoryItemInstance::GetItemDefinition()
{
	return itemDefinition;
}

int32 ULomInventoryItemInstance::GetTagStackCount(FGameplayTag Tag)
{
	return TagStackContainer.GetTagStackCount(Tag);
}

void ULomInventoryItemInstance::AddTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.AddTagStack(Tag, Count);
}

void ULomInventoryItemInstance::OverrideTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.OverrideTagStack(Tag, Count);
}

bool ULomInventoryItemInstance::ContainsTagStack(FGameplayTag Tag)
{
	return TagStackContainer.ContainsTagStack(Tag);
}

void ULomInventoryItemInstance::RemoveTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.RemoveTagStack(Tag, Count);
}

void ULomInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TagStackContainer);
	DOREPLIFETIME(ThisClass, Amount);

}

bool ULomInventoryItemInstance::IsSupportedForNetworking() const
{
	return true;
}
