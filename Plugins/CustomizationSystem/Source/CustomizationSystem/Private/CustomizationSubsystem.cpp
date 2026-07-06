// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizationSubsystem.h"
#include "CustomizerComponent.h"

UCustomizerComponent* UCustomizationSubsystem::GetCurrentCustomizerComponent()
{
    return currentCustomizerComponent;
}

AActor* UCustomizationSubsystem::GetCurrentItem()
{
    return currentItem;
}

void UCustomizationSubsystem::SetCurrentItem(AActor* item)
{
    currentItem = item;
    currentCustomizerComponent = item->GetComponentByClass<UCustomizerComponent>();
}
