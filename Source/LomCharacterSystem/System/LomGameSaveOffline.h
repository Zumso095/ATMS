// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/LomGameSavePlatform.h"
#include "GameFramework/SaveGame.h"

#include "LomGameSaveOffline.generated.h"


UCLASS()
class ULomFloatGameSave : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	float Value;
};
UCLASS()
class ULomIntGameSave : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	int32 Value;
};
UCLASS()
class ULomStringGameSave : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	FString Value;
};
UCLASS()
class ULomBoolGameSave : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	bool Value;
};
/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameSaveOffline : public ULomGameSavePlatform
{
	GENERATED_BODY()

public:
	
	virtual void SaveFloat(const FString& Key, float Value) override;

	
	virtual float GetFloat(const FString& Key) override;

	
	virtual void SaveInt(const FString& Key, int32 Value) override;

	
	virtual int32 GetInt(const FString& Key) override;

	
	virtual void SaveString(const FString& Key, const FString& Value) override;

	
	virtual FString GetString(const FString& Key) override;


	virtual void SaveBool(const FString& Key, bool Value) override;


	virtual bool GetBool(const FString& Key) override;
	
};
