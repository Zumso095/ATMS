// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/LomGameplayAbility.h"

#include "LomGameplayAbility_FromEquipment.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameplayAbility_FromEquipment : public ULomGameplayAbility
{
	GENERATED_BODY()
public:
	ULomGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	ULomEquipmentItemInstance* GetAssociatedEquipment() const;

private:
	UPROPERTY()
	mutable ULomEquipmentItemInstance* cachedAssociatedEquipment;

};
