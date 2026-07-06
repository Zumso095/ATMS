// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LomGamePhaseSubsystem.generated.h"

/**
 * 
 */
class ULomGameplayAbility_GamePhase;
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGamePhaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StartPhase(TSubclassOf<ULomGameplayAbility_GamePhase> GamePhase);
};
