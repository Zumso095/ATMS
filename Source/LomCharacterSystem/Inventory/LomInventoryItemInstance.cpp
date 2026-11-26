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
	const int32 oldCount = TagStackContainer.GetTagStackCount(Tag);
	TagStackContainer.AddTagStack(Tag, Count);
	BroadcastIfChanged(Tag, oldCount);
}

void ULomInventoryItemInstance::OverrideTagStack(FGameplayTag Tag, int32 Count)
{
	const int32 oldCount = TagStackContainer.GetTagStackCount(Tag);
	TagStackContainer.OverrideTagStack(Tag, Count);
	BroadcastIfChanged(Tag, oldCount);
}

bool ULomInventoryItemInstance::ContainsTagStack(FGameplayTag Tag)
{
	return TagStackContainer.ContainsTagStack(Tag);
}

void ULomInventoryItemInstance::RemoveTagStack(FGameplayTag Tag, int32 Count)
{
	const int32 oldCount = TagStackContainer.GetTagStackCount(Tag);
	TagStackContainer.RemoveTagStack(Tag, Count);
	BroadcastIfChanged(Tag, oldCount);
}

void ULomInventoryItemInstance::BroadcastIfChanged(FGameplayTag Tag, int32 OldCount)
{
	const int32 newCount = TagStackContainer.GetTagStackCount(Tag);
	if (newCount != OldCount)
	{
		OnTagStackChanged.Broadcast(this, Tag, newCount, OldCount);
	}
}

void ULomInventoryItemInstance::OnRep_TagStackContainer(const FGameplayTagStackContainer& OldContainer)
{
	// Container getters are non-const, so work on copies.
	FGameplayTagStackContainer oldCopy = OldContainer;
	TArray<FGameplayTagStack> newStacks = TagStackContainer.GetAllTagStack();
	TArray<FGameplayTagStack> oldStacks = oldCopy.GetAllTagStack();

	// Tags that exist now: changed or newly added
	for (const FGameplayTagStack& stack : newStacks)
	{
		const int32 oldCount = oldCopy.GetTagStackCount(stack.Tag);
		if (stack.Count != oldCount)
		{
			OnTagStackChanged.Broadcast(this, stack.Tag, stack.Count, oldCount);
		}
	}

	// Tags that disappeared (count now reads as 0)
	for (const FGameplayTagStack& stack : oldStacks)
	{
		if (!TagStackContainer.ContainsTagStack(stack.Tag) && stack.Count != 0)
		{
			OnTagStackChanged.Broadcast(this, stack.Tag, 0, stack.Count);
		}
	}
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
