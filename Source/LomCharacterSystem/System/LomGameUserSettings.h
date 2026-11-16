// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "LomGameUserSettings.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFOVChangedSignature, float, NewValue);


class APlayerController;

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

	UPROPERTY(Config)
	float FieldOfView = 90.0f;

public: 

	virtual void ApplyNonResolutionSettings() override;

	UFUNCTION(BlueprintCallable)
	void SetMotionBlurEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetAmbienceVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void SetUIVolume(float Volume);

	UFUNCTION(BlueprintCallable)
	void ApplyFieldOfView(APlayerController* PlayerController) const;

	UFUNCTION(BlueprintCallable)
	void SetFieldOfView(float Value, APlayerController* PlayerController);

	UPROPERTY(BlueprintAssignable)
	FOnFOVChangedSignature OnFOVChanged;


	UFUNCTION(BlueprintCallable, BlueprintPure)
	const bool GetMotionBlurEnabled() { return bMotionBlurEnabled; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetFieldOfView() { return FieldOfView; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetMusicVolume() { return MusicVolume; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetMasterVolume() { return MasterVolume; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetAnbienceVolume() { return AmbienceVolume; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetUIVolume() { return UIVolume; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float GetSFXVolume() { return SFXVolume; };

	UFUNCTION(BlueprintCallable)
	void ApplyAidioSettings();

	UFUNCTION(BlueprintCallable)
	void ApplyMotionBlurSettings();

private: 




};
