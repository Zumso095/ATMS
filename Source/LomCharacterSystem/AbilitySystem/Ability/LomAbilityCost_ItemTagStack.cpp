// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/LomAbilityCost_ItemTagStack.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "Equipment/LomGameplayAbility_FromEquipment.h"
#include "Inventory/LomInventoryItemInstance.h"

bool ULomAbilityCost_ItemTagStack::CheckCost(const ULomGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
    if (!Ability)
    {
        return false;
    }
   // UE_LOG(LogTemp, Display, TEXT("test"));


    if (const ULomGameplayAbility_FromEquipment* abilityFromEquipment = Cast<ULomGameplayAbility_FromEquipment>(Ability))
    {
        // UE_LOG(LogTemp, Error, TEXT("associated object"));
        if (abilityFromEquipment->GetAssociatedEquipment())
        {
            ULomInventoryItemInstance* instance = abilityFromEquipment->GetAssociatedEquipment()->InventoryItemInstance;
  
            if (instance && instance->GetTagStackCount(Tag) >= Count)
            {
                return true;
            }
        }
       
    }
   
    
    return false;
}

void ULomAbilityCost_ItemTagStack::ApplyCost(const ULomGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    if (const ULomGameplayAbility_FromEquipment* abilityFromEquipment = Cast<ULomGameplayAbility_FromEquipment>(Ability))
    {
        if (abilityFromEquipment->GetAssociatedEquipment())
        {
            ULomInventoryItemInstance* instance = abilityFromEquipment->GetAssociatedEquipment()->InventoryItemInstance;
            if (instance)
            {
                instance->RemoveTagStack(Tag, Count);
            }
        }
    }
   
   
}
