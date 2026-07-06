// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "InputAction.h"
#include "LomInputConfig.generated.h"


USTRUCT()
struct FLomInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere)
	UInputAction* InputAction = nullptr;
};
/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FLomInputAction> InputActions;
	
};
