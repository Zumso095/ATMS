// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LomGameplayAbility_FromEquipment.h"

ULomGameplayAbility_FromEquipment::ULomGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	cachedAssociatedEquipment = nullptr;

}

ULomEquipmentItemInstance* ULomGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	//if (AssociatedItem)
	//{
	//	return AssociatedItem;
	//}
	//else
	//{
	if (!IsInstantiated())
	{
		UE_LOG(LogTemp, Error, TEXT("Ability is not instantiated"));
		//return nullptr;
	}
	if (cachedAssociatedEquipment != nullptr)
	{
		return cachedAssociatedEquipment;
	}

	if (FGameplayAbilitySpec* abilitySpec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		//UE_LOG(LogTemp, Error, TEXT("current spec"));

		ULomEquipmentItemInstance* item = Cast<ULomEquipmentItemInstance>(abilitySpec->SourceObject.Get());
		if (item)
		{
			//UE_LOG(LogTemp, Error, TEXT("item"));

			//AssociatedItem = item;
			cachedAssociatedEquipment = item;
			return item;
		}
	}

	//}
	return nullptr;
}