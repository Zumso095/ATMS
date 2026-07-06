// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LomAbilitySystemComponent.generated.h"



//class FGameplayTag;
/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable)
	void PressInputWithTag(FGameplayTag InputTag);

	UFUNCTION(BlueprintCallable)
	void ReleaseInputWithTag(FGameplayTag InputTag);

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
};
