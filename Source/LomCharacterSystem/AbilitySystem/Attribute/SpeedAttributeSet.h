// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/LomAttributeSet.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"

#include "SpeedAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API USpeedAttributeSet : public ULomAttributeSet
{
	GENERATED_BODY()
public:

	ATTRIBUTE_ACCESSORS(USpeedAttributeSet, Speed);
	//ATTRIBUTE_ACCESSORS(USpeedAttributeSet, SpeedMax);

protected:

	UFUNCTION()
	void OnRep_Speed(FGameplayAttributeData& value);

	
private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Speed, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Speed;
};
