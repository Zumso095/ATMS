// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/LomInventoryItemDefinition.h"
#include "Inventory/LomInventoryItemInstance.h"
//#include "Containers/Tuple.h"

#include "LomInventoryManagerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemRemovedDelegate, ULomInventoryItemInstance*, InventoryItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemAddedDelegate, ULomInventoryItemInstance*, InventoryItemInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInventoryItemCountChangedDelegate, ULomInventoryItemInstance*, InventoryItemInstance,int32, OldValue,int32, NewValue);


USTRUCT(BlueprintType)
struct FInventoryItemsList
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	ULomInventoryItemInstance* Instance = nullptr;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOMCHARACTERSYSTEM_API ULomInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULomInventoryManagerComponent();

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemRemovedDelegate OnInventoryItemRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemAddedDelegate OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemCountChangedDelegate OnInventoryItemCountChanged;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

public:
	UFUNCTION(BlueprintCallable)
	ULomInventoryItemInstance* AddItemDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition,int32 Count = 1, bool Stack = true);

	UFUNCTION(BlueprintCallable)
	ULomInventoryItemInstance* AddItemInstance(ULomInventoryItemInstance* ItemInstance, bool Stack = true);


	UFUNCTION(BlueprintCallable)
	TArray<FInventoryItemsList>& GetAllItems();

	UFUNCTION(BlueprintCallable)
	int32 GetInventoryItemsAmount();

	UFUNCTION(BlueprintCallable)
	ULomInventoryItemInstance* GetInventoryItemInstanceByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool Contain(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemByDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition, int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemByDefinitionFull(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemByInstance(ULomInventoryItemInstance* ItemInstance, int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItemByInstanceFull(ULomInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable)
	void RemoveAllItems();

	/*UFUNCTION(BlueprintCallable)
	bool FindItemByInstance(ULomInventoryItemInstance* ItemInstance);*/

	UFUNCTION(BlueprintCallable)
	ULomInventoryItemInstance* FindItemByDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition,bool &Success );

	UFUNCTION(BlueprintCallable)
	ULomInventoryItemInstance* TransferInstance(ULomInventoryItemInstance* ItemInstance, ULomInventoryManagerComponent* TargetInventory, bool Stack = false);

	UFUNCTION(BlueprintCallable)
	int32 GetSlotsLimit();

	UFUNCTION(BlueprintCallable)
	void SetSlotsLimit(int32 Count);

private:
	UPROPERTY(Replicated)
	TArray<FInventoryItemsList> ItemsList;

	UPROPERTY(EditDefaultsOnly)
	int32 slotsLimit = 100;


};
