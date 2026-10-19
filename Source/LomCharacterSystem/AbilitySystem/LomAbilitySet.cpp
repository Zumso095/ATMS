// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LomAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Equipment/LomEquipmentItemInstance.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/Ability/LomGameplayAbility.h"

void ULomAbilitySet::GiveAbility(UAbilitySystemComponent* abilitySystemComponent, ULomEquipmentItemInstance* AssociatedItemInstance, FLomAbilitySetGrantedHandles* OutGrantedHandles) const
{
	if (!abilitySystemComponent || !abilitySystemComponent->IsOwnerActorAuthoritative())
	{
		return;
	}
	if (OutGrantedHandles)
	{
		// Reusing a grant record first removes its previous grants from their original ASC.
		OutGrantedHandles->TakeFromAbilitySystem();
		OutGrantedHandles->AbilitySystem = abilitySystemComponent;
	}
	for (const FGrantedAbilityItem& grantedAbility : Abilities)
	{
		if (grantedAbility.AbilityClass)
		{
			ULomGameplayAbility* abilityCDO = grantedAbility.AbilityClass->GetDefaultObject<ULomGameplayAbility>();
			FGameplayAbilitySpec abilitySpec(abilityCDO);
			abilitySpec.GetDynamicSpecSourceTags().AddTag(grantedAbility.InputTag);
			if (AssociatedItemInstance)
			{
				abilitySpec.SourceObject = AssociatedItemInstance;

			}

			const FGameplayAbilitySpecHandle& abilityHandle = abilitySystemComponent->GiveAbility(abilitySpec);
			//	abilitySystemComponent->AbilityLocalInputPressed

			if (OutGrantedHandles)
			{
				OutGrantedHandles->AbilityHandles.Add(abilityHandle);
			}
		}
	
		
	}


	for (const FGrantedAttributeItem& grantedAttributeSet : Attributes)
	{
		if (grantedAttributeSet.AttributeSet)
		{
			UAttributeSet* attributeSet = NewObject<UAttributeSet>(abilitySystemComponent->GetOwner(), grantedAttributeSet.AttributeSet);
			abilitySystemComponent->AddAttributeSetSubobject(attributeSet);
			if (OutGrantedHandles)
			{
				OutGrantedHandles->AddedAttributeSets.Add(attributeSet);
			}
		}
	}


	for (const FGrantedEffectItem& grantedEffect : Effects)
	{
		if (grantedEffect.Effect)
		{
			const UGameplayEffect* effectCDO = grantedEffect.Effect->GetDefaultObject<UGameplayEffect>();
			const FActiveGameplayEffectHandle& effectHandle = abilitySystemComponent->ApplyGameplayEffectToSelf(effectCDO, 1, abilitySystemComponent->MakeEffectContext());
			if (OutGrantedHandles)
			{
				OutGrantedHandles->EffectHandles.Add(effectHandle);
			}
		}
	}
}

void FLomAbilitySetGrantedHandles::TakeFromAbilitySystem()
{
	UAbilitySystemComponent* abilitySystemComponent = AbilitySystem.Get();
	if (abilitySystemComponent && !abilitySystemComponent->IsOwnerActorAuthoritative())
	{
		return;
	}

	// Clear this record before callbacks from ability removal can re-enter cleanup.
	TArray<FGameplayAbilitySpecHandle> RemovedAbilities = MoveTemp(AbilityHandles);
	TArray<FActiveGameplayEffectHandle> RemovedEffects = MoveTemp(EffectHandles);
	TArray<TObjectPtr<UAttributeSet>> RemovedAttributes = MoveTemp(AddedAttributeSets);
	AbilitySystem.Reset();
	if (!abilitySystemComponent)
	{
		return;
	}
	for (const FGameplayAbilitySpecHandle& abilityHandle : RemovedAbilities)
	{
		abilitySystemComponent->ClearAbility(abilityHandle);
	}
	for (const FActiveGameplayEffectHandle& effectHandle : RemovedEffects)
	{
		abilitySystemComponent->RemoveActiveGameplayEffect(effectHandle);
	}
	for (UAttributeSet* attributeSet : RemovedAttributes)
	{
		abilitySystemComponent->RemoveSpawnedAttribute(attributeSet);
	}
}
