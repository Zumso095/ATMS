// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LomQuickBarComponent.h"
#include "Equipment/LomEquipmentManagerComponent.h"
#include "Inventory/LomInventoryItemInstance.h"
#include "Inventory/LomInventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
ULomQuickBarComponent::ULomQuickBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	Slots.AddDefaulted(SlotsCount);
	// ...
}


// Called when the game starts
void ULomQuickBarComponent::BeginPlay()
{
	Super::BeginPlay();

	ULomInventoryManagerComponent* inventory = GetOwner()->GetComponentByClass<ULomInventoryManagerComponent>();
	if (inventory)
	{
		//UE_LOG(LogTemp, Error, TEXT("inventory manager"));

		inventory->OnInventoryItemRemoved.AddUniqueDynamic(this, &ULomQuickBarComponent::OnInventoryItemRemoved);
	}

	// ...
	
}

void ULomQuickBarComponent::OnInventoryItemRemoved(ULomInventoryItemInstance* InventoryItemInstance)
{
	UE_LOG(LogTemp, Display, TEXT("removing item from slots"));
	//UnEquipItem(InventoryItemInstance);
	//NotifyRemovedFromSlot(InventoryItemInstance, 0);
	RemoveFromSlot(InventoryItemInstance);
}

void ULomQuickBarComponent::OnRep_SlotsChanged(const TArray<ULomInventoryItemInstance*>& OldItems)
{
	for (ULomInventoryItemInstance* OldInventoryItemInstance : OldItems)
	{
		if (OldInventoryItemInstance && !Slots.Contains(OldInventoryItemInstance))
		{
			//UE_LOG(LogTemp, Error, TEXT("unequipped  %s"), *equippedItem->GetName());

			NotifySlotEmpty.Broadcast(OldInventoryItemInstance, OldItems.Find(OldInventoryItemInstance));
			UE_LOG(LogTemp, Error, TEXT("item removed: index == %i"), OldItems.Find(OldInventoryItemInstance));

		}
	}
	for (ULomInventoryItemInstance* inventoryItemInstance : Slots)
	{
		if (inventoryItemInstance && !OldItems.Contains(inventoryItemInstance))
		{
		
			NotifySlotAdded.Broadcast(inventoryItemInstance, Slots.Find(inventoryItemInstance));
			UE_LOG(LogTemp, Error, TEXT("item added: index == %i"), Slots.Find(inventoryItemInstance));


		}
	}
	
}

// Called every frame
void ULomQuickBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULomQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, currentSlotIndex);
	DOREPLIFETIME(ThisClass, Slots);
}

void ULomQuickBarComponent::NextItem()
{
	
}

void ULomQuickBarComponent::PreviousItem()
{
}

int32 ULomQuickBarComponent::AddItemToSlot(ULomInventoryItemInstance* InventoryItemInstance)
{


	for (int32 index = 0; index < Slots.Num(); index++)
	{
		if (Slots[index] == nullptr)
		{
			
			Slots[index] = InventoryItemInstance;
			NotifySlotAdded.Broadcast(InventoryItemInstance, index);

			return index;
		}
	}

	return -1;
}

int32 ULomQuickBarComponent::AddItemToSlotByIndex(ULomInventoryItemInstance* InventoryItemInstance, int32 Index)
{
	if (Index <= Slots.Num())
	{
		Slots.Insert(InventoryItemInstance, Index);
		NotifySlotAdded.Broadcast(InventoryItemInstance, Index);
		return Index;

	}
	

	//NotifyAddedToSlot(InventoryItemInstance, Index);
	return -1;
}

void ULomQuickBarComponent::RemoveFromSlot(ULomInventoryItemInstance* InventoryItemInstance)
{
	for (int32 index = 0; index < Slots.Num();index++)
	{
		if (Slots[index] == InventoryItemInstance)
		{
		//	NotifyRemovedFromSlot(InventoryItemInstance, index);
			NotifySlotEmpty.Broadcast(InventoryItemInstance, index);

			//Slots.RemoveAt(index);
			Slots[index] = nullptr;
			return;
		}
	}
}

ULomEquipmentItemInstance* ULomQuickBarComponent::ActivateSlotByIndex(int32 Index)
{
	ULomEquipmentManagerComponent* equipmentManager = GetOwner()->GetComponentByClass<ULomEquipmentManagerComponent>();
	if (equipmentManager && Slots.IsValidIndex(Index) && Slots[Index])
	{
		if (Slots.IsValidIndex(currentSlotIndex))
		{
			equipmentManager->UnEquipItem(Slots[currentSlotIndex]);

		}
		currentSlotIndex = Index;	
		return equipmentManager->EquipItem(Slots[Index]);
	}
	return nullptr;
}

void ULomQuickBarComponent::DeactivateActiveSlot()
{
	ULomEquipmentManagerComponent* equipmentManager = GetOwner()->GetComponentByClass<ULomEquipmentManagerComponent>();
	if (equipmentManager && Slots.IsValidIndex(currentSlotIndex) && Slots[currentSlotIndex])
	{	

			equipmentManager->UnEquipItem(Slots[currentSlotIndex]);	
			Slots[currentSlotIndex] = nullptr;
	}
	
}

void ULomQuickBarComponent::DropItemFromActiveSlot()
{
	ULomInventoryManagerComponent* inventory = GetOwner()->GetComponentByClass<ULomInventoryManagerComponent>();
	if (inventory && Slots.IsValidIndex(currentSlotIndex) && Slots[currentSlotIndex])
	{
		inventory->RemoveItemByInstance(Slots[currentSlotIndex]);		
	}
}

ULomInventoryItemInstance* ULomQuickBarComponent::GetItemFromActiveSlot()
{
	//ULomEquipmentManagerComponent* equipmentManager = GetOwner()->GetComponentByClass<ULomEquipmentManagerComponent>();
	if (Slots.IsValidIndex(currentSlotIndex) && Slots[currentSlotIndex])
	{
		return Slots[currentSlotIndex];	
		
	}
	return nullptr;
}

ULomInventoryItemInstance* ULomQuickBarComponent::GetItemFromSlot(int32 Index)
{
	if (Slots.IsValidIndex(Index) && Slots[Index])
	{
		return Slots[Index];

	}
	
	return nullptr;
}

int32 ULomQuickBarComponent::GetCurrentSlotIndex()
{
	return currentSlotIndex;
}



//void ULomQuickBarComponent::NotifyAddedToSlot_Implementation(ULomInventoryItemInstance* InventoryItemInstance, int32 Index)
//{
//	//GetNetMode()
//	FString netMode;
//		switch (GetNetMode())
//		{
//		case ENetMode::NM_Client :
//			netMode = "Client";
//			break;
//		case ENetMode::NM_DedicatedServer:
//			netMode = "DedicatedServer";
//			break;
//		case ENetMode::NM_ListenServer:
//			netMode = "ListenServer";
//			break;
//		case ENetMode::NM_Standalone:
//			netMode = "Standalone";
//			break;
//		default:
//			netMode = "None";
//
//			break;
//		}
//	if(InventoryItemInstance)
//	{
//		UE_LOG(LogTemp, Error, TEXT("NetMode == %s, notifyed added: inventoryItemInstance name ==  %s"),*netMode,*InventoryItemInstance->GetName());
//
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("NetMode == %s,notifyed added: NullPtr"), *netMode);
//
//	}
//
//	NotifySlotAdded.Broadcast(InventoryItemInstance, Index);
//}

