// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LomGameSavePlatform.h"
#include "LomGameSaveOffline.h"

#include "LomGameSaveSubsystem.generated.h"

/**
 * 
 */


UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()


	
public:
	UFUNCTION(BlueprintCallable)
	void SaveFloat(const FString& Key, float Value, TSubclassOf<ULomGameSavePlatform> Platform);

	UFUNCTION(BlueprintCallable)
	float GetFloat(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform);

	UFUNCTION(BlueprintCallable)
	void SaveInt(const FString& Key, int32 Value, TSubclassOf<ULomGameSavePlatform> Platform);

	UFUNCTION(BlueprintCallable)
	int32 GetInt(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform);

	UFUNCTION(BlueprintCallable)
	void SaveString(const FString& Key,const FString& Value, TSubclassOf<ULomGameSavePlatform> Platform);

	UFUNCTION(BlueprintCallable)
	FString GetString(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform);

	UFUNCTION(BlueprintCallable)
	void SaveBool(const FString& Key, bool Value, TSubclassOf<ULomGameSavePlatform> Platform);

	UFUNCTION(BlueprintCallable)
	bool GetBool(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform);

private:
	UPROPERTY()
	TMap<TSubclassOf<ULomGameSavePlatform>, TObjectPtr<ULomGameSavePlatform>> saveGamePlatforms;

	ULomGameSavePlatform* GetSaveGamePlatform(TSubclassOf<ULomGameSavePlatform> PlatformClass);
	
};
