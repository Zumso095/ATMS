// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilitySystem/LomAbilitySet.h"
#include "LomPawnData.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> DefaultPawn;

	UPROPERTY(EditDefaultsOnly)
	TArray<ULomAbilitySet*> AbilitySets;
};
