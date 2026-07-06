// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/Ability/LomGameplayAbility.h"
#include "InputAction.h"

#include "LomAbilitySet.generated.h"

class UAbilitySystemComponent;
class ULomEquipmentItemInstance;
/**
 * 
 */
USTRUCT()
struct FGrantedAbilityItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULomGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	/*UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction;*/
};

USTRUCT()
struct FGrantedEffectItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> Effect;
};

USTRUCT()
struct FGrantedAttributeItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> AttributeSet;
};

UCLASS()
class LOMCHARACTERSYSTEM_API ULomAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	void GiveAbility(UAbilitySystemComponent* abilitySystemComponent, ULomEquipmentItemInstance* AssociatedItemInstance = nullptr);
	void ClearAbility(UAbilitySystemComponent* abilitySystemComponent);

	UPROPERTY(EditDefaultsOnly)
	TArray<FGrantedAbilityItem> Abilities;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGrantedEffectItem> Effects;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGrantedAttributeItem> Attributes;

private:


	TArray<FGameplayAbilitySpecHandle> AbilitiesHandles;
	TArray<FActiveGameplayEffectHandle> EffectHandles;
	TArray<TObjectPtr<UAttributeSet>> AddedAttributeSets;

};
