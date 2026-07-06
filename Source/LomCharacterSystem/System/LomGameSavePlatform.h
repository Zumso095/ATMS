// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LomGameSavePlatform.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LOMCHARACTERSYSTEM_API ULomGameSavePlatform : public UObject
{
	GENERATED_BODY()
	
public:
	ULomGameSavePlatform();

	virtual void SaveFloat(const FString& Key, float Value) {};
	virtual float GetFloat(const FString& Key) { return 0.0f; };
	
	virtual void SaveInt(const FString& Key, int32 Value) {};
	virtual int32 GetInt(const FString& Key) { return 0; };

	virtual void SaveString(const FString& Key,const FString& Value) {};
	virtual FString GetString(const FString& Key) { return ""; };

	virtual void SaveBool(const FString& Key, bool Value) {};
	virtual bool GetBool(const FString& Key) { return false; };
};
