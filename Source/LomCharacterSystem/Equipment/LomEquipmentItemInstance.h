// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameplayTags.h"
#include "LomEquipmentItemInstance.generated.h"

class ULomInventoryItemDefinition;
class ULomInventoryItemInstance;
class ULomAbilitySet;
class ULomEquipmentItemDefinition;
class ULomEquipmentItemInstance;
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class LOMCHARACTERSYSTEM_API ULomEquipmentItemInstance : public UObject
{
	GENERATED_BODY()
	


public:

	//ULomInventoryItemInstance* InventoryItemInstance;

	virtual UWorld* GetWorld() const override;
	AActor* GetOwnerActor();
	APawn* GetOwnerPawn();

	void InitInstance(ULomInventoryItemDefinition* InventoryItemDefinition,ULomInventoryItemInstance* InventoryItemInstance);
	void InitInstanceWithParams(ULomInventoryItemDefinition* InventoryItemDefinition, ULomInventoryItemInstance* InventoryItemInstance,FTransform Offset);
	void DeInitInstance();

	void SpawnActor(const ULomEquipmentItemDefinition* EquipmentDefinition, AActor* owner);
	void SpawnActorWithParams(const ULomEquipmentItemDefinition* EquipmentDefinition, AActor* owner, FTransform Transform);
	void RemoveActors();

	void AddAbilitySet(ULomAbilitySet* AbilitySet, AActor* Target);
	void RemoveAbilitySet(AActor* Target);

	void AddWidgets();
	void RemoveAddedWidgets();

	void Equipped();
	void UnEquipped();

	void ChangeSocket(FName SocketName,FTransform Transform, int32 ActorIndex);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Equipped"))
	void K2_Equipped();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UnEquipped"))
	void K2_UnEquipped();

	virtual bool IsSupportedForNetworking() const override;

public:
	UPROPERTY(Replicated,BlueprintReadWrite)
	TArray<AActor*> SpawnedActors;

	ULomAbilitySet* attachedAbilitySet;

	TArray<UUserWidget*> addedWidgets;
	//ULomAbilitySet* currentAbilitySet;
	/*UPROPERTY(ReplicatedUsing = OnRep_AttachedInventoryItemInstance)
	ULomInventoryItemInstance* attachedInventoryItemInstance;*/

	FGameplayTag ItemType;
	

public:

	UPROPERTY(ReplicatedUsing = OnRep_InventoryItemDefinition)
	ULomInventoryItemDefinition* InventoryItemDefinition;

	UPROPERTY(Replicated, BlueprintReadOnly)
	ULomInventoryItemInstance* InventoryItemInstance;

	UFUNCTION()
	void OnRep_InventoryItemDefinition();
};
