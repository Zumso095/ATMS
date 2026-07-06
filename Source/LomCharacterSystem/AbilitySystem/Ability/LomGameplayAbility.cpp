// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/LomGameplayAbility.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "AbilitySystem/Ability/LomAbilityCost.h"


void ULomGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	AbilityAdded();
	
	if (ActivationPolicy == EAbilityActivationPolicy::OnSpawned && !Spec.IsActive())
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
	}
}

bool ULomGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;

	}

	for (const TObjectPtr<ULomAbilityCost>& abilityCost : CustomCosts)
	{
		if (abilityCost)
		{
			if (!abilityCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
			{
				return false;
			}
		}
		
	}

	return true;
}

void ULomGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	for (const TObjectPtr<ULomAbilityCost>& abilityCost : CustomCosts)
	{
		if (abilityCost)
		{
			abilityCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);

		}
	}
}
void ULomGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	AbilityRemoved();
}
#if WITH_EDITOR
EDataValidationResult ULomGameplayAbility::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		//Context.AddError(NSLOCTEXT("Lyra", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		UE_LOG(LogTemp, Error, TEXT("Ability is not valid"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
	
}
#endif
ULomGameplayAbility::ULomGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
}


