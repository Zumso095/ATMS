// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/Ability/LomGameplayAbility.h"
#include "InputAction.h"

#include "LomAbilitySet.generated.h"

class UAbilitySystemComponent;
class ULomEquipmentItemInstance;

// Runtime grants belong to one equipment instance, never to the shared data asset.
USTRUCT()
struct FLomAbilitySetGrantedHandles
{
	GENERATED_BODY()

	void TakeFromAbilitySystem();

	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystem;

	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	TArray<FActiveGameplayEffectHandle> EffectHandles;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAttributeSet>> AddedAttributeSets;
};
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
	void GiveAbility(UAbilitySystemComponent* abilitySystemComponent, ULomEquipmentItemInstance* AssociatedItemInstance = nullptr, FLomAbilitySetGrantedHandles* OutGrantedHandles = nullptr) const;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGrantedAbilityItem> Abilities;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGrantedEffectItem> Effects;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGrantedAttributeItem> Attributes;

};
