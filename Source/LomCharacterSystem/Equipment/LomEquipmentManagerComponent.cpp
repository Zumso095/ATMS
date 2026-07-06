// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LomEquipmentManagerComponent.h"
#include "Inventory/LomInventoryItemInstance.h"
#include "Inventory/LomInventoryItemDefinition.h"
#include "Inventory/InventoryFragment_EquippableItem.h"
#include "Equipment/LomEquipmentItemDefinition.h"
#include "Vehicle/LomWheeledVehiclePawn.h"
#include "Equipment/LomEquipmentItemInstance.h"
#include "Inventory/LomInventoryManagerComponent.h"
#include <Net/UnrealNetwork.h>
#include "Engine/ActorChannel.h"




ULomEquipmentManagerComponent::ULomEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);

}

void ULomEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(ThisClass, EquippedItems);
}

void ULomEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ULomInventoryManagerComponent* inventory = GetOwner()->GetComponentByClass<ULomInventoryManagerComponent>();
	if (inventory)
	{
		//UE_LOG(LogTemp, Error, TEXT("inventory manager"));

		inventory->OnInventoryItemRemoved.AddUniqueDynamic(this, &ULomEquipmentManagerComponent::OnInventoryItemRemoved);
	}

}
bool ULomEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool wroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (ULomEquipmentItemInstance* equippedItem : EquippedItems)
	{
		if (equippedItem)
		{
			wroteSomething |= Channel->ReplicateSubobject(equippedItem, *Bunch, *RepFlags);

		}
	}
	return wroteSomething;
}
void ULomEquipmentManagerComponent::OnRep_EquippedItems(const TArray<ULomEquipmentItemInstance*>& OldItems)
{
	//UE_LOG(LogTemp, Error, TEXT("OnRep Function works"));

	for (ULomEquipmentItemInstance* equippedItem : EquippedItems)
	{
		if (equippedItem && !OldItems.Contains(equippedItem))
		{
			equippedItem->Equipped();
			//UE_LOG(LogTemp, Error, TEXT("equip item %s"),*equippedItem->GetName());

		}
	}
	for (ULomEquipmentItemInstance* equippedItem : OldItems)
	{
		if (equippedItem && !EquippedItems.Contains(equippedItem))
		{
			//UE_LOG(LogTemp, Error, TEXT("unequipped  %s"), *equippedItem->GetName());

			equippedItem->UnEquipped();
		}
	}
}
void ULomEquipmentManagerComponent::OnInventoryItemRemoved(ULomInventoryItemInstance* InventoryItemInstance)
{
//	UE_LOG(LogTemp, Display, TEXT("removing item"));
	UnEquipItem(InventoryItemInstance);
}

ULomEquipmentItemInstance* ULomEquipmentManagerComponent::EquipItem(ULomInventoryItemInstance* InventoryItem)
{
	if (GetOwner()->HasAuthority())
	{ 
		if (InventoryItem)
		{			
			ULomInventoryItemDefinition* inventoryItemDefinition = GetMutableDefault<ULomInventoryItemDefinition>(InventoryItem->GetItemDefinition());
			const UInventoryFragment_EquippableItem* equippableItem = Cast<UInventoryFragment_EquippableItem>( inventoryItemDefinition->FindFragment(UInventoryFragment_EquippableItem::StaticClass()));
		
			if (equippableItem && equippableItem->EquipmentDefinition)
			{
				const ULomEquipmentItemDefinition* equipmentItemDefinition = GetDefault< ULomEquipmentItemDefinition>(equippableItem->EquipmentDefinition);
				ULomEquipmentItemInstance* equipmnetItemInstance = NewObject<ULomEquipmentItemInstance>(GetOwner(), equipmentItemDefinition->EquipmentItemInstanceClass);
				equipmnetItemInstance->InitInstance(inventoryItemDefinition,InventoryItem);
				AddReplicatedSubObject(equipmnetItemInstance);
				EquippedItems.Add(equipmnetItemInstance);

				return equipmnetItemInstance;
			}

		}
	}
	
	return nullptr;
}

ULomEquipmentItemInstance* ULomEquipmentManagerComponent::EquipItemToSlot(FGameplayTag Slot, ULomInventoryItemInstance* InventoryItem)
{
	if (GetOwner()->HasAuthority())
	{
		if (InventoryItem && Slot.IsValid())
		{
			for (int32 index = 0; index < EquippedItems.Num(); index++)
			{
				if (EquippedItems[index]->ItemType == Slot)
				{
					EquippedItems[index]->DeInitInstance();
					RemoveReplicatedSubObject(EquippedItems[index]);
					//EquippedItems[index]->ConditionalBeginDestroy(); don't need it because it will be destroyed by garbage collector
					EquippedItems.RemoveAt(index);
					break;
				}
			}

			ULomInventoryItemDefinition* inventoryItemDefinition = GetMutableDefault<ULomInventoryItemDefinition>(InventoryItem->GetItemDefinition());
			const UInventoryFragment_EquippableItem* equippableItem = Cast<UInventoryFragment_EquippableItem>(inventoryItemDefinition->FindFragment(UInventoryFragment_EquippableItem::StaticClass()));

			if (equippableItem && equippableItem->EquipmentDefinition)
			{
				const ULomEquipmentItemDefinition* equipmentItemDefinition = GetDefault< ULomEquipmentItemDefinition>(equippableItem->EquipmentDefinition);
				ULomEquipmentItemInstance* equipmnetItemInstance = NewObject<ULomEquipmentItemInstance>(GetOwner(), equipmentItemDefinition->EquipmentItemInstanceClass);
				equipmnetItemInstance->InitInstance(inventoryItemDefinition, InventoryItem);
				AddReplicatedSubObject(equipmnetItemInstance);
				equipmnetItemInstance->ItemType = Slot;
				EquippedItems.Add(equipmnetItemInstance);

				return equipmnetItemInstance;
			}

		}
	}

	return nullptr;
}

ULomEquipmentItemInstance* ULomEquipmentManagerComponent::EquipItemWithParams(ULomInventoryItemInstance* InventoryItem, FTransform Transform)
{
	if (GetOwner()->HasAuthority())
	{
		if (InventoryItem)
		{
			 ULomInventoryItemDefinition* inventoryItemDefinition = GetMutableDefault<ULomInventoryItemDefinition>(InventoryItem->GetItemDefinition());
			const UInventoryFragment_EquippableItem* equippableItem = Cast<UInventoryFragment_EquippableItem>(inventoryItemDefinition->FindFragment(UInventoryFragment_EquippableItem::StaticClass()));

			if (equippableItem && equippableItem->EquipmentDefinition)
			{
				const ULomEquipmentItemDefinition* equipmentItemDefinition = GetDefault< ULomEquipmentItemDefinition>(equippableItem->EquipmentDefinition);
				ULomEquipmentItemInstance* equipmnetItemInstance = NewObject<ULomEquipmentItemInstance>(GetOwner(), equipmentItemDefinition->EquipmentItemInstanceClass);
				equipmnetItemInstance->InitInstanceWithParams(inventoryItemDefinition,InventoryItem,Transform);
				AddReplicatedSubObject(equipmnetItemInstance);
				EquippedItems.Add(equipmnetItemInstance);

				return equipmnetItemInstance;
			}

		}
	}

	return nullptr;
}

void ULomEquipmentManagerComponent::UnEquipItem(ULomInventoryItemInstance* InventoryItem)
{
	if (GetOwner()->HasAuthority())
	{
		if (InventoryItem)
		{
			for (int32 index = 0; index < EquippedItems.Num(); index++)
			{
				if (EquippedItems[index]->InventoryItemInstance == InventoryItem)
				{
					EquippedItems[index]->DeInitInstance();
					RemoveReplicatedSubObject(EquippedItems[index]);
					//EquippedItems[index]->ConditionalBeginDestroy(); don't need it because it will be destroyed by garbage collector
					EquippedItems.RemoveAt(index);
				}
			}

		}
	}
	
}

void ULomEquipmentManagerComponent::UnEquipItemFromSlot(FGameplayTag Slot)
{
	if (GetOwner()->HasAuthority())
	{
		if (Slot.IsValid())
		{
			for (int32 index = 0; index < EquippedItems.Num(); index++)
			{
				if (EquippedItems[index]->ItemType == Slot)
				{
					EquippedItems[index]->DeInitInstance();
					RemoveReplicatedSubObject(EquippedItems[index]);
					//EquippedItems[index]->ConditionalBeginDestroy(); don't need it because it will be destroyed by garbage collector
					EquippedItems.RemoveAt(index);
					break;
				}
			}

		}
	}
}

TArray<ULomEquipmentItemInstance*>& ULomEquipmentManagerComponent::GetAllEquippedItems()
{
	// TODO: insert return statement here
	return EquippedItems;
}

ULomEquipmentItemInstance* ULomEquipmentManagerComponent::FindEquipmentItemInstance(ULomInventoryItemInstance* InventoryItemInstance)
{
	if (InventoryItemInstance)
	{
		for (int32 index = 0; index < EquippedItems.Num(); index++)
		{
			if (EquippedItems[index]->InventoryItemInstance == InventoryItemInstance)
			{
				//EquippedItems[index]->DeInitInstance();
				///RemoveReplicatedSubObject(EquippedItems[index]);
				//EquippedItems[index]->ConditionalBeginDestroy(); don't need it because it will be destroyed by garbage collector
				//EquippedItems.RemoveAt(index);
				return EquippedItems[index];
			}
		}

	}
	return nullptr;
}

void ULomEquipmentManagerComponent::ChangeSocket(ULomEquipmentItemInstance* EquipmentInstance, FName SocketName, FTransform Transform, int32 ActorIndex)
{
	EquipmentInstance->ChangeSocket(SocketName, Transform, ActorIndex);
}

ULomEquipmentItemInstance* ULomEquipmentManagerComponent::GetEquipmentFromSlot(FGameplayTag Slot)
{
	if (Slot.IsValid())
	{
		for (int32 index = 0; index < EquippedItems.Num(); index++)
		{
			if (EquippedItems[index]->ItemType == Slot)
			{
				return EquippedItems[index];
				
			}
		}
	}
	return nullptr;
}


