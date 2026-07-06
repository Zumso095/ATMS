// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/LomAttributeSet.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"

#include "StaminaAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API UStaminaAttributeSet : public ULomAttributeSet
{
	GENERATED_BODY()
	
public:

	ATTRIBUTE_ACCESSORS(UStaminaAttributeSet, Stamina);

protected:

	UFUNCTION()
	void OnRep_Stamina(FGameplayAttributeData& value);

private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
};
