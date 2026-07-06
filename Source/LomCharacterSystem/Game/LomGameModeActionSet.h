// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFeatureAction.h"


#include "LomGameModeActionSet.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameModeActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UGameFeatureAction*> GameFeatureActions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> PluginsToEnable;
	
};
