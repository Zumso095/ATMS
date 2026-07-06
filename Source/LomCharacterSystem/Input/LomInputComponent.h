// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "LomInputComponent.generated.h"


class ULomInputConfig;
/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void BindInputConfig(ULomInputConfig* Config);

	void PressInputTag(FGameplayTag InputTag);
	void ReleaseInputTag(FGameplayTag InputTag);
};
