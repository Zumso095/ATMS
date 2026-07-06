// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LomGameSaveSubsystem.h"



void ULomGameSaveSubsystem::SaveFloat(const FString& Key, float Value, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);
	platform->SaveFloat(Key, Value);
	
}

float ULomGameSaveSubsystem::GetFloat(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);
	return platform->GetFloat(Key);
}

void ULomGameSaveSubsystem::SaveInt(const FString& Key, int32 Value, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);
	platform->SaveInt(Key, Value);
}

int32 ULomGameSaveSubsystem::GetInt(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);
	
	return platform->GetInt(Key);
}

void ULomGameSaveSubsystem::SaveString(const FString& Key, const FString& Value, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);
	platform->SaveString(Key, Value);
}

FString ULomGameSaveSubsystem::GetString(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);

	return platform->GetString(Key);
}

void ULomGameSaveSubsystem::SaveBool(const FString& Key, bool Value, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);
	platform->SaveBool(Key, Value);
}

bool ULomGameSaveSubsystem::GetBool(const FString& Key, TSubclassOf<ULomGameSavePlatform> Platform)
{
	ULomGameSavePlatform* platform = GetSaveGamePlatform(Platform);

	return platform->GetBool(Key);
}

ULomGameSavePlatform* ULomGameSaveSubsystem::GetSaveGamePlatform(TSubclassOf<ULomGameSavePlatform> PlatformClass)
{
	ULomGameSavePlatform* platform = saveGamePlatforms.FindOrAdd(PlatformClass);
	if (platform == nullptr)
	{
		if (PlatformClass) 
		{
			platform = NewObject<ULomGameSavePlatform>(this, PlatformClass);

		}
		else 
		{
			platform = NewObject<ULomGameSaveOffline>();
			UE_LOG(LogTemp,Error,TEXT("Missing PlatformClass. It was set default Offline platfom"))

		}
	}
	return platform;
}
