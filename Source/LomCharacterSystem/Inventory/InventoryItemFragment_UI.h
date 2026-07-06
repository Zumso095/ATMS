// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/LomInventoryItemDefinition.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTags.h"

#include "InventoryItemFragment_UI.generated.h"

/**
 * 
 */
USTRUCT()
struct FUIFragmentItemContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AnchorPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> WidgetClass;


};

UCLASS()
class LOMCHARACTERSYSTEM_API UInventoryItemFragment_UI : public ULomInventoryItemFragment
{
	GENERATED_BODY()
public:
	void AddWidgets();
	void RemoveWidgets();

	UPROPERTY(EditDefaultsOnly)
	TArray<FUIFragmentItemContainer> WidgetsToAdd;

private:

	TArray<UUserWidget*> addedWidgets;
};
