// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/LomAttributeSet.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"

#include "HealthAttributeSet.generated.h"

DECLARE_MULTICAST_DELEGATE_FiveParams(FOnAttributeChangeDelegate,AActor* /*Instigator*/,const FGameplayEffectSpec* /*EffectSpec*/, float/*Effect Magnitude*/, float /*OldValue*/, float/*NewValue*/)
DECLARE_MULTICAST_DELEGATE(FOnAttributeResetDelegate)
/**
 * 
 */
UCLASS(BlueprintType)
class LOMCHARACTERSYSTEM_API UHealthAttributeSet : public ULomAttributeSet
{
	GENERATED_BODY()
public:

    ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Health);
    ATTRIBUTE_ACCESSORS(UHealthAttributeSet, HealthMax);
    ATTRIBUTE_ACCESSORS(UHealthAttributeSet, Mana);
    ATTRIBUTE_ACCESSORS(UHealthAttributeSet, ManaMax);

    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

    mutable FOnAttributeChangeDelegate OnDead;
    mutable FOnAttributeChangeDelegate OnHealthChanged;
    mutable FOnAttributeResetDelegate OnReset;

protected:

    UFUNCTION()
    void OnRep_Health(FGameplayAttributeData& value);
    
    UFUNCTION()
    void OnRep_HealthMax(FGameplayAttributeData& value);

    UFUNCTION()
    void OnRep_Mana(FGameplayAttributeData& value);

    UFUNCTION()
    void OnRep_ManaMax(FGameplayAttributeData& value);

private:

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthMax, meta = (AllowPrivateAccess = true))
    FGameplayAttributeData HealthMax;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, meta = (AllowPrivateAccess = true))
    FGameplayAttributeData Mana;

    UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaMax, meta = (AllowPrivateAccess = true))
    FGameplayAttributeData ManaMax;

    float healthBeforeChange = 0;
};
