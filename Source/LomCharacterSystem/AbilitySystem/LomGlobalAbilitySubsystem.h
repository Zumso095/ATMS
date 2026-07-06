// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LomAbilitySystemComponent.h"
#include "LomGlobalAbilitySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGlobalAbilitySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public: 
	void AddAbilitySystemComponent(ULomAbilitySystemComponent* ASC);
	void RemoveAbilitySystemComponent(ULomAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToAll(TSubclassOf<UGameplayEffect> Effect);
	void ApplyEffectsForNewASC(ULomAbilitySystemComponent* abilitySystemComponent);

	UFUNCTION(BlueprintCallable)
	void RemoveGameplayEffectFromAll(TSubclassOf<UGameplayEffect> Effect);



private:

	TArray<ULomAbilitySystemComponent*> abilitySystemComponents;
	TArray<TSubclassOf<UGameplayEffect>> gameplayEffectsForApply;
};
