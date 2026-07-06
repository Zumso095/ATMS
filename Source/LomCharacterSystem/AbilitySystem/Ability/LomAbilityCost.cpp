// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/LomAbilityCost.h"

bool ULomAbilityCost::CheckCost(const ULomGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    return true;
}

void ULomAbilityCost::ApplyCost(const ULomGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
}
