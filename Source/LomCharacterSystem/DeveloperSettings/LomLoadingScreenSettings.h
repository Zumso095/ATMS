// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "LomLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "LoadingScreen"))
class LOMCHARACTERSYSTEM_API ULomLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	ULomLoadingScreenSettings();

	UPROPERTY(config, EditAnywhere, Category = Configuration)
	FText MenuLevelName;

	UPROPERTY(config, EditAnywhere, Category = Display, meta = (MetaClass = "/Script/UMG.UserWidget"))
	FSoftClassPath LoadingScreenWidget;

	UPROPERTY(config, EditAnywhere, Category = Display, meta = (MetaClass = "/Script/UMG.UserWidget"))
	FSoftClassPath StartupWidget;

	UPROPERTY(config, EditAnywhere, Category = Configuration)
	bool HoldLoadingScreenAdditionalSecsEvenInEditor = true;

	UPROPERTY(config, EditAnywhere, Category = Configuration)
	float HoldLoadingScreenAdditionalSecs = 2.0f;

	UPROPERTY(config, EditAnywhere, Category = Configuration)
	bool ForceTickLoadingScreenEvenInEditor = true;

	UPROPERTY(config, EditAnywhere, Category = Configuration)
	int32 LoadingScreenZOrder = 10000;
	
};
