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



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOMCHARACTERSYSTEM_API ULomEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	ULomEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;

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
