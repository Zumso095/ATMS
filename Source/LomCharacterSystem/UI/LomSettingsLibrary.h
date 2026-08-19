// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LomSettingsLibrary.generated.h"

/**
 * Reads and writes settings values through ULomGameSaveSubsystem under one shared key
 * convention, plus the display helpers the resolution row needs.
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomSettingsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Save slot key used for a setting ("Setting.<SettingID>"). */
	UFUNCTION(BlueprintPure, Category = "Lom|Settings")
	static FString MakeSettingKey(FName SettingID);

	/** True once this setting has been saved at least once. */
	UFUNCTION(BlueprintCallable, Category = "Lom|Settings", meta = (WorldContext = "WorldContextObject"))
	static bool HasSavedSetting(const UObject* WorldContextObject, FName SettingID);

	/** Saved value for the setting, or DefaultValue when it has never been saved. */
	UFUNCTION(BlueprintCallable, Category = "Lom|Settings", meta = (WorldContext = "WorldContextObject"))
	static float ResolveSavedFloat(const UObject* WorldContextObject, FName SettingID, float DefaultValue);

	/** Index of the saved option within Options, or DefaultIndex when nothing valid was saved. */
	UFUNCTION(BlueprintCallable, Category = "Lom|Settings", meta = (WorldContext = "WorldContextObject"))
	static int32 ResolveSavedListIndex(const UObject* WorldContextObject, FName SettingID, const TArray<FName>& Options, int32 DefaultIndex);

	UFUNCTION(BlueprintCallable, Category = "Lom|Settings", meta = (WorldContext = "WorldContextObject"))
	static void SaveFloatSetting(const UObject* WorldContextObject, FName SettingID, float Value);

	UFUNCTION(BlueprintCallable, Category = "Lom|Settings", meta = (WorldContext = "WorldContextObject"))
	static void SaveNameSetting(const UObject* WorldContextObject, FName SettingID, FName Value);

	/** Persist every entry of both maps. Called when the player confirms with Apply. */
	UFUNCTION(BlueprintCallable, Category = "Lom|Settings", meta = (WorldContext = "WorldContextObject"))
	static void SavePendingSettings(const UObject* WorldContextObject, const TMap<FName, float>& FloatSettings, const TMap<FName, FName>& ListSettings);

	/** Resolution options that fit on this display, formatted as "1920 x 1080". */
	UFUNCTION(BlueprintCallable, Category = "Lom|Settings")
	static TArray<FName> GetAvailableResolutionOptions();

	/** Parse an option string; also accepts the legacy "1920 x1080" spelling. */
	UFUNCTION(BlueprintPure, Category = "Lom|Settings")
	static bool ParseResolutionOption(FName Option, FIntPoint& OutResolution);

	/** Shrink a resolution so it can never exceed the usable desktop area. */
	UFUNCTION(BlueprintPure, Category = "Lom|Settings")
	static FIntPoint ClampResolutionToDesktop(FIntPoint Resolution);
};
