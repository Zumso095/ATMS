// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LomGameSaveOffline.h"
#include "Kismet/GameplayStatics.h"


void ULomGameSaveOffline::SaveFloat(const FString& Key, float Value)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomFloatGameSave* floatSaveGame = Cast<ULomFloatGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (floatSaveGame)
		{
			floatSaveGame->Value = Value;
			UGameplayStatics::SaveGameToSlot(floatSaveGame, Key, 0);
		}
	}
	else
	{
		ULomFloatGameSave* floatSaveGame = Cast<ULomFloatGameSave>(UGameplayStatics::CreateSaveGameObject(ULomFloatGameSave::StaticClass()));
		floatSaveGame->Value = Value;
		UGameplayStatics::SaveGameToSlot(floatSaveGame, Key, 0);
	}
}

float ULomGameSaveOffline::GetFloat(const FString& Key)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomFloatGameSave* floatSaveGame = Cast<ULomFloatGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (floatSaveGame)
		{

			return floatSaveGame->Value;
		}

	}
	return 0.0f;
}

void ULomGameSaveOffline::SaveInt(const FString& Key, int32 Value)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomIntGameSave* intSaveGame = Cast<ULomIntGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (intSaveGame)
		{
			intSaveGame->Value = Value;
			UGameplayStatics::SaveGameToSlot(intSaveGame, Key, 0);
		}
	}
	else
	{
		ULomIntGameSave* intSaveGame = Cast<ULomIntGameSave>(UGameplayStatics::CreateSaveGameObject(ULomIntGameSave::StaticClass()));
		intSaveGame->Value = Value;
		UGameplayStatics::SaveGameToSlot(intSaveGame, Key, 0);
	}
}

int32 ULomGameSaveOffline::GetInt(const FString& Key)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomIntGameSave* intSaveGame = Cast<ULomIntGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (intSaveGame)
		{

			return intSaveGame->Value;
		}

	}
	return 0;
}

void ULomGameSaveOffline::SaveString(const FString& Key, const FString& Value)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomStringGameSave* stringSaveGame = Cast<ULomStringGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (stringSaveGame)
		{
			stringSaveGame->Value = Value;
			UGameplayStatics::SaveGameToSlot(stringSaveGame, Key, 0);
		}
	}
	else
	{
		ULomStringGameSave* stringSaveGame = Cast<ULomStringGameSave>(UGameplayStatics::CreateSaveGameObject(ULomStringGameSave::StaticClass()));
		stringSaveGame->Value = Value;
		UGameplayStatics::SaveGameToSlot(stringSaveGame, Key, 0);
	}
}

FString ULomGameSaveOffline::GetString(const FString& Key)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomStringGameSave* stringSaveGame = Cast<ULomStringGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (stringSaveGame)
		{

			return stringSaveGame->Value;
		}

	}
	return "";
}

void ULomGameSaveOffline::SaveBool(const FString& Key, bool Value)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomBoolGameSave* boolSaveGame = Cast<ULomBoolGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (boolSaveGame)
		{
			boolSaveGame->Value = Value;
			UGameplayStatics::SaveGameToSlot(boolSaveGame, Key, 0);
		}
	}
	else
	{
		ULomBoolGameSave* boolSaveGame = Cast<ULomBoolGameSave>(UGameplayStatics::CreateSaveGameObject(ULomBoolGameSave::StaticClass()));
		boolSaveGame->Value = Value;
		UGameplayStatics::SaveGameToSlot(boolSaveGame, Key, 0);
	}
}

bool ULomGameSaveOffline::GetBool(const FString& Key)
{
	if (UGameplayStatics::DoesSaveGameExist(Key, 0))
	{
		ULomBoolGameSave* boolSaveGame = Cast<ULomBoolGameSave>(UGameplayStatics::LoadGameFromSlot(Key, 0));
		if (boolSaveGame)
		{

			return boolSaveGame->Value;
		}

	}
	return false;
}
