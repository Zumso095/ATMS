// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"

#include "LomWorldSettings.generated.h"

/**
 * 
 */
class ULomGameModeDefinition;

UCLASS()
class LOMCHARACTERSYSTEM_API ALomWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
	ULomGameModeDefinition* DefaultGameModeDefinition;

public:
	ULomGameModeDefinition* GetDefaultGameModeDefinition() { return DefaultGameModeDefinition; }
};
