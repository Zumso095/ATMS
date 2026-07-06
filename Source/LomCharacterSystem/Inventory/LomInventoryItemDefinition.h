// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LomInventoryItemDefinition.generated.h"

class ULomInventoryItemInstance;


UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class LOMCHARACTERSYSTEM_API ULomInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(ULomInventoryItemInstance* ItemInstance) { OnInstanceCreatedEvent(ItemInstance); };

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)

	void OnInstanceCreatedEvent(ULomInventoryItemInstance* ItemInstance);
};

/**
 * 
 */
UCLASS(Blueprintable, Const, Abstract)
class LOMCHARACTERSYSTEM_API ULomInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	const ULomInventoryItemFragment* FindFragment(TSubclassOf<ULomInventoryItemFragment> fragment) const;
public:
	UPROPERTY(EditAnywhere,Instanced)
	TArray<ULomInventoryItemFragment*> Fragments;

	
};
