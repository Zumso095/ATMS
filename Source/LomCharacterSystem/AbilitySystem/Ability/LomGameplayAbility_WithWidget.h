// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/LomGameplayAbility.h"
#include "GameplayTags.h"
#include "LomGameplayAbility_WithWidget.generated.h"

/**
 * 
 */
class UUserWidget;
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameplayAbility_WithWidget : public ULomGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag,TSubclassOf<UUserWidget>> Widgets;

	//
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	//~

private:
	TArray<UUserWidget*> addedWidgets;
};
