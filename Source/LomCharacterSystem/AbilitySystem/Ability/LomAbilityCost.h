// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Ability/LomGameplayAbility.h"
#include "LomAbilityCost.generated.h"

/**
 * 
 */
UCLASS(Abstract,EditInlineNew,DefaultToInstanced)
class LOMCHARACTERSYSTEM_API ULomAbilityCost : public UObject
{
	GENERATED_BODY()
	
public:

	virtual bool CheckCost(const ULomGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const ;
	virtual void ApplyCost(const ULomGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const ;
	
};
