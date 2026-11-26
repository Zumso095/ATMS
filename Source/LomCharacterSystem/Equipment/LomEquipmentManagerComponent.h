// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTags.h"
#include "LomEquipmentManagerComponent.generated.h"

class ULomEquipmentItemInstance;

/**
 * 
 */
class ULomInventoryItemInstance;

// Fired from ULomEquipmentItemInstance::Equipped/UnEquipped: on the server right away, on clients from OnRep_EquippedItems.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChangedDelegate, ULomEquipmentItemInstance*, EquipmentInstance, ULomInventoryItemInstance*, InventoryItemInstance);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOMCHARACTERSYSTEM_API ULomEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	ULomEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChangedDelegate OnItemEquipped;

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChangedDelegate OnItemUnequipped;

	UFUNCTION(BlueprintCallable)
	ULomEquipmentItemInstance* EquipItem(ULomInventoryItemInstance* InventoryItem);

	UFUNCTION(BlueprintCallable)
	ULomEquipmentItemInstance* EquipItemToSlot(FGameplayTag Slot,ULomInventoryItemInstance* InventoryItem);


	UFUNCTION(BlueprintCallable)
	ULomEquipmentItemInstance* EquipItemWithParams(ULomInventoryItemInstance* InventoryItem, FTransform Transform);

	UFUNCTION(BlueprintCallable)
	void UnEquipItem(ULomInventoryItemInstance* InventoryItem);

	UFUNCTION(BlueprintCallable)
	void UnEquipItemFromSlot(FGameplayTag Slot);

	UFUNCTION(BlueprintCallable)
	TArray<ULomEquipmentItemInstance*>& GetAllEquippedItems();

	UFUNCTION(BlueprintCallable)
	ULomEquipmentItemInstance* FindEquipmentItemInstance(ULomInventoryItemInstance* InventoryItemInstance);

	UFUNCTION(BlueprintCallable)
	void ChangeSocket(ULomEquipmentItemInstance* EquipmentInstance, FName SocketName, FTransform Transform, int32 ActorIndex = 0);

	UFUNCTION(BlueprintCallable)
	ULomEquipmentItemInstance* GetEquipmentFromSlot(FGameplayTag Slot);

public:
	//if you don't mark uboject with uproperty it will destroy with garbage collector
	UPROPERTY(ReplicatedUsing="OnRep_EquippedItems")
	TArray<ULomEquipmentItemInstance*> EquippedItems;

	

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	UFUNCTION()
	void OnRep_EquippedItems(const TArray<ULomEquipmentItemInstance*>& OldItems);

private:

	UFUNCTION()
	void OnInventoryItemRemoved(ULomInventoryItemInstance* InventoryItemInstance);
};
