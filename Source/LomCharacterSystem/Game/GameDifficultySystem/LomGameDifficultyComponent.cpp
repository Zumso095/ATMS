// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameDifficultySystem/LomGameDifficultyComponent.h"

float ULomGameDifficultyComponent::GetGameDifficultyCoefficient(float Level)
{
    if (BaseDifficultyCurve)
    {
        return BaseDifficultyCurve->GetFloatValue(Level);
    }
    return 0.0f;
}

float ULomGameDifficultyComponent::GetDifficultyByTag(const FGameplayTag Tag, float InTime)
{
    //UCurveFloat* curve = CustomDifficultyCurves.FindChecked(Tag);
    if (CustomDifficultyCurves.Find(Tag))
    {
        return (*CustomDifficultyCurves.Find(Tag))->GetFloatValue(InTime);
    }
    
    return 0.0f;
}
