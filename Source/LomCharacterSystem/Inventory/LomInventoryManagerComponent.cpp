// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/LomInventoryManagerComponent.h"
#include <Net/UnrealNetwork.h>
#include "Engine/ActorChannel.h"

DEFINE_LOG_CATEGORY_STATIC(InventoryLog, All, All)


// Sets default values for this component's properties
ULomInventoryManagerComponent::ULomInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void ULomInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULomInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool ULomInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool wroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (FInventoryItemsList& inventoryItem : ItemsList)
	{
		if (inventoryItem.Instance)
		{
			wroteSomething |= Channel->ReplicateSubobject(inventoryItem.Instance, *Bunch, *RepFlags);

		}
	}
	return wroteSomething;
}



ULomInventoryItemInstance* ULomInventoryManagerComponent::AddItemDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition, int32 Count, bool Stack)
{
	AActor* owner = GetOwner();
	if (!owner || !owner->HasAuthority() || !ItemDefinition)
	{
		return nullptr;
	}

	if (Count > 0)
	{
		if (Stack)
		{
			for (int32 index = 0; index < ItemsList.Num(); index++)
			{
				if (ItemsList[index].Instance->GetItemDefinition() == ItemDefinition)
				{
					int32 oldValue = ItemsList[index].Instance->Amount;
					ItemsList[index].Instance->Amount += Count;				
					OnInventoryItemCountChanged.Broadcast(ItemsList[index].Instance, oldValue, ItemsList[index].Instance->Amount);
					return ItemsList[index].Instance;
				}
			}
		}
		if (slotsLimit >= ItemsList.Num())
		{
			//UE_LOG(LogTemp, Warning, TEXT("addddddddd"));

			FInventoryItemsList& item = ItemsList.AddDefaulted_GetRef();
			item.Instance = NewObject<ULomInventoryItemInstance>(GetOwner());
			item.Instance->SetItemDefinition(ItemDefinition);
			item.Instance->Amount = Count;
			for (ULomInventoryItemFragment* fragment : GetDefault<ULomInventoryItemDefinition>(ItemDefinition)->Fragments)
			{
				fragment->OnInstanceCreated(item.Instance);
			}
			AddReplicatedSubObject(item.Instance);
			OnInventoryItemAdded.Broadcast(item.Instance);
			return item.Instance;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Inventory: No free slots"));
		}
		
		
	}
	//UE_LOG(LogTemp, Warning, TEXT("hhhhhh"));

	return nullptr;
}

ULomInventoryItemInstance* ULomInventoryManagerComponent::AddItemInstance(ULomInventoryItemInstance* ItemInstance, bool Stack)
{
	AActor* owner = GetOwner();
	if (!owner || !owner->HasAuthority())
	{

		return nullptr;
	}
	//if (ItemInstance->Amount <= 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Inventory: You tried to add itemInstans with 0 amount"));

	//	return nullptr;
	//}
	if (Stack)
	{
		for (int32 index = 0; index < ItemsList.Num(); index++)
		{
			if (ItemsList[index].Instance->GetItemDefinition() == ItemInstance->GetItemDefinition())
			{
				int32 oldValue = ItemsList[index].Instance->Amount;
				ItemsList[index].Instance->Amount += ItemInstance->Amount;
				OnInventoryItemCountChanged.Broadcast(ItemsList[index].Instance, oldValue, ItemsList[index].Instance->Amount);
				return ItemsList[index].Instance;
			}
		}

	}
	if (slotsLimit >= ItemsList.Num()) 
	{
		FInventoryItemsList& item = ItemsList.AddDefaulted_GetRef();
		item.Instance = ItemInstance;
		AddReplicatedSubObject(item.Instance);
		OnInventoryItemAdded.Broadcast(item.Instance);
		return item.Instance;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory: No free slots"));

	}
	return nullptr;
	
		
	

	
}

TArray<FInventoryItemsList>& ULomInventoryManagerComponent::GetAllItems()
{
	// TODO: insert return statement here
	return ItemsList;
}

int32 ULomInventoryManagerComponent::GetInventoryItemsAmount()
{
	return ItemsList.Num();
}

ULomInventoryItemInstance* ULomInventoryManagerComponent::GetInventoryItemInstanceByIndex(int32 Index)
{
	
	if (ItemsList.IsValidIndex(Index))
	{
		
		return ItemsList[Index].Instance;
	}
	return nullptr;
}

bool ULomInventoryManagerComponent::Contain(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition)
{
	for (FInventoryItemsList& itemDef : ItemsList)
	{
		if (itemDef.Instance->GetItemDefinition() == ItemDefinition)
		{
			return true;
		}
	}
	return false;
}

bool ULomInventoryManagerComponent::RemoveItemByDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition, int32 Count)
{
	AActor* owner = GetOwner();
	if (!owner || !owner->HasAuthority())
	{
		return false;
	}
	for (int32 index = 0; index < ItemsList.Num(); index++)
	{
		if (ItemsList[index].Instance->GetItemDefinition() == ItemDefinition && ItemsList[index].Instance->Amount >= Count)
		{
			int32 oldValue = ItemsList[index].Instance->Amount;
			ItemsList[index].Instance->Amount -= Count;
			if (ItemsList[index].Instance->Amount <= 0)
			{
				//ItemsList[index].Instance->ConditionalBeginDestroy();
				OnInventoryItemCountChanged.Broadcast(ItemsList[index].Instance, oldValue, ItemsList[index].Instance->Amount);
				OnInventoryItemRemoved.Broadcast(ItemsList[index].Instance);
				RemoveReplicatedSubObject(ItemsList[index].Instance);
				ItemsList.RemoveAt(index);

				return true;
			}
			else
			{
				OnInventoryItemCountChanged.Broadcast(ItemsList[index].Instance, oldValue, ItemsList[index].Instance->Amount);
				return true;

			}

			
		}
	}
	return false;
}

bool ULomInventoryManagerComponent::RemoveItemByDefinitionFull(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition)
{
	AActor* owner = GetOwner();
	if (!owner || !owner->HasAuthority())
	{
		return false;
	}
	for (int32 index = 0; index < ItemsList.Num(); index++)
	{
		if (ItemsList[index].Instance->GetItemDefinition() == ItemDefinition)
		{		
				OnInventoryItemRemoved.Broadcast(ItemsList[index].Instance);
				RemoveReplicatedSubObject(ItemsList[index].Instance);
				ItemsList.RemoveAt(index);
				return true;
		}
	}
	return false;
}

bool ULomInventoryManagerComponent::RemoveItemByInstance(ULomInventoryItemInstance* ItemInstance, int32 Count)
{
	AActor* owner = GetOwner();
	if (!owner || !owner->HasAuthority())
	{
		return false;
	}
	for (int32 index = 0; index < ItemsList.Num(); index++)
	{
		
		if (ItemsList[index].Instance == ItemInstance && ItemsList[index].Instance->Amount >= Count)
		{
			int32 oldValue = ItemsList[index].Instance->Amount;
			ItemsList[index].Instance->Amount -= Count;
			if (ItemsList[index].Instance->Amount <= 0)
			{
				//ItemsList[index].Instance->ConditionalBeginDestroy();
				OnInventoryItemCountChanged.Broadcast(ItemsList[index].Instance, oldValue, ItemsList[index].Instance->Amount);
				OnInventoryItemRemoved.Broadcast(ItemsList[index].Instance);
				RemoveReplicatedSubObject(ItemsList[index].Instance);
				ItemsList.RemoveAt(index);

				return true;
			}
			else
			{
				OnInventoryItemCountChanged.Broadcast(ItemsList[index].Instance, oldValue, ItemsList[index].Instance->Amount);
				return true;

			}


		}
	}
	return false;
}

bool ULomInventoryManagerComponent::RemoveItemByInstanceFull(ULomInventoryItemInstance* ItemInstance)
{
	AActor* owner = GetOwner();
	if (!owner || !owner->HasAuthority())
	{
		return false;
	}
	for (int32 index = 0; index < ItemsList.Num(); index++)
	{
		if (ItemsList[index].Instance == ItemInstance)
		{
				OnInventoryItemRemoved.Broadcast(ItemsList[index].Instance);
				RemoveReplicatedSubObject(ItemsList[index].Instance);
				ItemsList.RemoveAt(index);

				return true;
			
		}
	}
	return false;
}

void ULomInventoryManagerComponent::RemoveAllItems()
{
	AActor* owner = GetOwner();
	if (!owner || !owner->HasAuthority())
	{
		return;
	}
	for (int32 index = 0; index < ItemsList.Num(); index++)
	{		
			OnInventoryItemRemoved.Broadcast(ItemsList[index].Instance);
			RemoveReplicatedSubObject(ItemsList[index].Instance);
			ItemsList.RemoveAt(index);
		
	}
	
}

//ULomInventoryItemInstance* ULomInventoryManagerComponent::FindItemByInstance(ULomInventoryItemInstance* ItemInstance,bool &Success)
//{
//	return nullptr;
//}




ULomInventoryItemInstance* ULomInventoryManagerComponent::FindItemByDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition,bool &Success )
{
	Success = false;
	for (FInventoryItemsList& itemDef : ItemsList)
	{
		if (itemDef.Instance->GetItemDefinition() == ItemDefinition)
		{
			Success = true;
			return itemDef.Instance;
		}
	}
	return nullptr;
}

ULomInventoryItemInstance* ULomInventoryManagerComponent::TransferInstance(ULomInventoryItemInstance* ItemInstance, ULomInventoryManagerComponent* TargetInventory, bool Stack)
{
	ULomInventoryItemInstance* TransferredItemInstance = TargetInventory->AddItemInstance(ItemInstance, Stack);
	RemoveItemByInstanceFull(ItemInstance);
	return TransferredItemInstance;
}

int32 ULomInventoryManagerComponent::GetSlotsLimit()
{
	return slotsLimit;
}

void ULomInventoryManagerComponent::SetSlotsLimit(int32 Count)
{
	slotsLimit = Count;
}

void ULomInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULomInventoryManagerComponent, ItemsList);
}

