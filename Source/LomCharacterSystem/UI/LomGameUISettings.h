// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPath.h"
#include "LomGameUISettings.generated.h"

/**
 * 
 */
class ULomGameUIPolicy;

UCLASS(config=Game, defaultconfig)
class LOMCHARACTERSYSTEM_API ULomGameUISettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:
	
	UPROPERTY(config, EditAnywhere, Category = Layout, meta = (AllowedClasses = "/Script/LomCharacterSystem.LomGameUIPolicy"))
	FSoftClassPath GameUIPolicy;
	
};
