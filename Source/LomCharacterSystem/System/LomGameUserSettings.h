// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "LomGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LOMCHARACTERSYSTEM_API ULomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	

private: 

	UPROPERTY(Config)
	float MasterVolume = 1.0f;

	UPROPERTY(Config)
	float MusicVolume = 1.0f;

	UPROPERTY(Config)
	float SFXVolume = 1.0f;

	UPROPERTY(Config)
	float AmbienceVolume = 1.0f;

	UPROPERTY(Config)
	float UIVolume = 1.0f;

	UPROPERTY(Config)
	bool bMotionBlurEnabled = true;

public: 

	virtual void ApplyNonResolutionSettings() override;

	UFUNCTION(BlueprintCallable)
	void SetMotionBlurEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetMusicVolumeVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetAmbienceVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetUIVolume(float Volume);


	UFUNCTION(BlueprintCallable, BlueprintPure)
	const bool GetMotionBlurEnabled() { return bMotionBlurEnabled; };

private: 

	void ApplyAidioSettings();

	void ApplyMotionBlurSettings();


};
