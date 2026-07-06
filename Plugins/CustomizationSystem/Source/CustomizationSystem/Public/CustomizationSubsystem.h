// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CustomizationSubsystem.generated.h"

/**
 * 
 */
class UCustomizerComponent;

UCLASS()
class CUSTOMIZATIONSYSTEM_API UCustomizationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private:
	UCustomizerComponent* currentCustomizerComponent;
	AActor* currentItem;

public:
	UFUNCTION(BlueprintCallable)
	UCustomizerComponent* GetCurrentCustomizerComponent();

	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentItem();

	UFUNCTION(BlueprintCallable)
	void SetCurrentItem(AActor* item);
};
