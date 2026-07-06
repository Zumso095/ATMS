// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LomAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Ability/LomGameplayAbility.h"

void ULomAbilitySet::GiveAbility(UAbilitySystemComponent* abilitySystemComponent, ULomEquipmentItemInstance* AssociatedItemInstance)
{
	for (const FGrantedAbilityItem& grantedAbility : Abilities)
	{
		ULomGameplayAbility* abilityCDO = grantedAbility.AbilityClass->GetDefaultObject<ULomGameplayAbility>();
		//abilityCDO->AssociatedItem = AssociatedItemInstance;
		if (grantedAbility.AbilityClass)
		{
			FGameplayAbilitySpec abilitySpec(abilityCDO);
			abilitySpec.GetDynamicSpecSourceTags().AddTag(grantedAbility.InputTag);
			if (AssociatedItemInstance)
			{
				abilitySpec.SourceObject = AssociatedItemInstance;

			}

			const FGameplayAbilitySpecHandle& abilityHandle = abilitySystemComponent->GiveAbility(abilitySpec);
			//	abilitySystemComponent->AbilityLocalInputPressed

			AbilitiesHandles.Add(abilityHandle);
		}
	
		
	}


	for (const FGrantedAttributeItem& grantedAttributeSet : Attributes)
	{
		if (grantedAttributeSet.AttributeSet)
		{
			UAttributeSet* attributeSet = NewObject<UAttributeSet>(abilitySystemComponent->GetOwner(), grantedAttributeSet.AttributeSet);
			abilitySystemComponent->AddAttributeSetSubobject(attributeSet);
			AddedAttributeSets.Add(attributeSet);
		}
	}


	for (const FGrantedEffectItem& grantedEffect : Effects)
	{
		if (grantedEffect.Effect)
		{
			const UGameplayEffect* effectCDO = grantedEffect.Effect->GetDefaultObject<UGameplayEffect>();
			const FActiveGameplayEffectHandle& effectHandle = abilitySystemComponent->ApplyGameplayEffectToSelf(effectCDO, 1, abilitySystemComponent->MakeEffectContext());
			EffectHandles.Add(effectHandle);
		}
	}
}

void ULomAbilitySet::ClearAbility(UAbilitySystemComponent* abilitySystemComponent)
{
	for (const FGameplayAbilitySpecHandle& abilityHandle : AbilitiesHandles)
	{
		abilitySystemComponent->ClearAbility(abilityHandle);
	}
	for (const FActiveGameplayEffectHandle& effectHandle : EffectHandles)
	{
		abilitySystemComponent->RemoveActiveGameplayEffect(effectHandle);
	}
	for (UAttributeSet* attributeSet : AddedAttributeSets)
	{
		abilitySystemComponent->RemoveSpawnedAttribute(attributeSet);
	}
	AbilitiesHandles.Reset();
	EffectHandles.Reset();
	AddedAttributeSets.Reset();
}
