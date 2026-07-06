// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddCuePath.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API UGameFeatureAction_AddCuePath : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	TArray<FDirectoryPath>& GetCuePathsToAdd() { return Paths; }

	UPROPERTY(EditAnywhere)
	TArray<FDirectoryPath> Paths;
};
