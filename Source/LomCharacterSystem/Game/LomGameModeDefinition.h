// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameFeatureAction.h"
#include "LomGameModeActionSet.h"
#include "Pawn/LomPawnData.h"

#include "LomGameModeDefinition.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameModeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/*UPROPERTY(EditAnywhere)
	FString name;*/
	UPROPERTY(EditDefaultsOnly)
	
	TObjectPtr<ULomPawnData>  PawnData;

	UPROPERTY(EditDefaultsOnly,Instanced)
	TArray<UGameFeatureAction*> GameFeatureActions;

	UPROPERTY(EditDefaultsOnly)
	TArray<ULomGameModeActionSet*> GameFeatureActionSets;

	UPROPERTY(EditDefaultsOnly)
	TArray<FString> PluginsToEnable;
};
