// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pawn/LomPawnData.h"

#include "LomGameFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	//Difficulty System
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static float GetGameDifficultyCoefficient(const UObject* WorldContextObject,float Level);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))

	static float GetDifficultyByTag(const UObject* WorldContextObject,const FGameplayTag Tag, float InTime);
	//~Difficulty System


	//Game Mode
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))

	static void SpawnActorFromPawnData(const UObject* WorldContextObject, ULomPawnData* PawnData, FTransform SpawnTransform);

	//~Game Mode

};
