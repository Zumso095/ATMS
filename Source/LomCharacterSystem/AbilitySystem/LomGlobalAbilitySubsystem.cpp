// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LomGlobalAbilitySubsystem.h"

void ULomGlobalAbilitySubsystem::AddAbilitySystemComponent(ULomAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		abilitySystemComponents.Add(ASC);
		ApplyEffectsForNewASC(ASC);
	}
}

void ULomGlobalAbilitySubsystem::RemoveAbilitySystemComponent(ULomAbilitySystemComponent* ASC)
{
	if (ASC)
	{
		for (int index = 0; index < abilitySystemComponents.Num(); index++)
		{
			if (abilitySystemComponents[index] == ASC) {
				abilitySystemComponents.RemoveAt(index);
				return;
			}
		}
	}
}

void ULomGlobalAbilitySubsystem::ApplyGameplayEffectToAll(TSubclassOf<UGameplayEffect> Effect)
{
	if (Effect)
	{
		gameplayEffectsForApply.Add(Effect);
		for (ULomAbilitySystemComponent* asc : abilitySystemComponents)
		{
			if (asc)
			{
				asc->BP_ApplyGameplayEffectToSelf(Effect, 1, asc->MakeEffectContext());

			}
		}
	}
}

void ULomGlobalAbilitySubsystem::ApplyEffectsForNewASC(ULomAbilitySystemComponent* abilitySystemComponent)
{
	for (TSubclassOf<UGameplayEffect>& effect : gameplayEffectsForApply)
	{
		abilitySystemComponent->BP_ApplyGameplayEffectToSelf(effect, 1, abilitySystemComponent->MakeEffectContext());

	}
}

void ULomGlobalAbilitySubsystem::RemoveGameplayEffectFromAll(TSubclassOf<UGameplayEffect> Effect)
{
	if (Effect)
	{
		gameplayEffectsForApply.Remove(Effect);

		for (ULomAbilitySystemComponent* asc : abilitySystemComponents)
		{
			asc->RemoveActiveGameplayEffectBySourceEffect(Effect, asc);
		}
	}
}
