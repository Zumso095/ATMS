// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Equipment/LomEquipmentItemInstance.h"
#include "LomGameplayAbility.generated.h"

class ULomAbilityCost;

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	OnPressed,
	OnSpawned
};
/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	ULomGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = "Activation")
	EAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly,Instanced, Category = "Costs")
	TArray<TObjectPtr<ULomAbilityCost>> CustomCosts;

	

	
	//
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//~

	UFUNCTION(BlueprintImplementableEvent, meta = (ScriptName = "ActivateAbility"))
	void AbilityAdded();

	UFUNCTION(BlueprintImplementableEvent)
	void AbilityRemoved();

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
