// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LomGameFunctionLibrary.h"
#include "Game/LomGameMode.h"
#include "Game/GameDifficultySystem/LomGameDifficultyComponent.h"

float ULomGameFunctionLibrary::GetGameDifficultyCoefficient(const UObject* WorldContextObject, float Level)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::Assert);
	if (world)
	{
		ULomGameDifficultyComponent* gdc = world->GetGameState()->FindComponentByClass<ULomGameDifficultyComponent>();
		if (gdc)
		{
			return gdc->GetGameDifficultyCoefficient(Level);
		}
	}
	return 0.0f;
}

float ULomGameFunctionLibrary::GetDifficultyByTag(const UObject* WorldContextObject,const FGameplayTag Tag, float InTime)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (world)
	{
		ULomGameDifficultyComponent* gdc = world->GetGameState()->FindComponentByClass<ULomGameDifficultyComponent>();
		if (gdc)
		{
			return gdc->GetDifficultyByTag(Tag,InTime);
		}
	}
	return 0.0f;
}

void ULomGameFunctionLibrary::SpawnActorFromPawnData(const UObject* WorldContextObject, ULomPawnData* PawnData, FTransform SpawnTransform)
{
	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (world)
	{
		ALomGameMode* gameMode = Cast<ALomGameMode>(world->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->SpawnActorFromPawnData(PawnData, SpawnTransform);
		}
	}
}
