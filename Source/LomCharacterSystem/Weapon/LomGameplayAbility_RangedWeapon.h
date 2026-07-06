// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment/LomGameplayAbility_FromEquipment.h"
#include "LomGameplayAbility_RangedWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameplayAbility_RangedWeapon : public ULomGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StartFire();


	
};
