// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LomQuickBarComponent.generated.h"

class ULomInventoryItemInstance;
class ULomEquipmentItemInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotAddedDelegate, ULomInventoryItemInstance*, InventoryItemInstance, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlotEmptyDelegate, ULomInventoryItemInstance*, InventoryItemInstance, int32, Index);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOMCHARACTERSYSTEM_API ULomQuickBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULomQuickBarComponent();
	UPROPERTY(BlueprintAssignable)
	FOnSlotAddedDelegate NotifySlotAdded;

	UPROPERTY(BlueprintAssignable)
	FOnSlotEmptyDelegate NotifySlotEmpty;

	UPROPERTY(EditAnywhere)
	int32 SlotsCount = 5;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	
	UFUNCTION(BlueprintCallable)
	void NextItem();
	
	UFUNCTION(BlueprintCallable)
	void PreviousItem();
	
	UFUNCTION(BlueprintCallable)
	int32 AddItemToSlot(ULomInventoryItemInstance* InventoryItemInstance);
	
	UFUNCTION(BlueprintCallable)
	int32 AddItemToSlotByIndex(ULomInventoryItemInstance* InventoryItemInstance, int32 Index);

	UFUNCTION(BlueprintCallable)
	void RemoveFromSlot(ULomInventoryItemInstance* InventoryItemInstance);
	
	UFUNCTION(BlueprintCallable)
	ULomEquipmentItemInstance* ActivateSlotByIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void DeactivateActiveSlot();

	UFUNCTION(BlueprintCallable)
	void DropItemFromActiveSlot();

	UFUNCTION(BlueprintCallable)
	ULomInventoryItemInstance* GetItemFromActiveSlot();

	UFUNCTION(BlueprintCallable)
	ULomInventoryItemInstance* GetItemFromSlot(int32 Index);

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentSlotIndex();

	

private:
	UPROPERTY(ReplicatedUsing = "OnRep_SlotsChanged")
	TArray<ULomInventoryItemInstance*> Slots;

	UPROPERTY(Replicated)
	int32 currentSlotIndex = 0;

private:

	UFUNCTION()
	void OnInventoryItemRemoved(ULomInventoryItemInstance* InventoryItemInstance);

	UFUNCTION()
	void OnRep_SlotsChanged(const TArray<ULomInventoryItemInstance*>& OldItems);
		
};
