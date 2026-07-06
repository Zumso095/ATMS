// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "Curves/CurveFloat.h"
#include "GameplayTags.h"

#include "LomGameDifficultyComponent.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameDifficultyComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	float GetGameDifficultyCoefficient(float Level);

	UFUNCTION(BlueprintCallable)
	float GetDifficultyByTag(const FGameplayTag Tag, float InTime);

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, UCurveFloat*> CustomDifficultyCurves;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> BaseDifficultyCurve;
};
