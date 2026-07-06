// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTags.h"
#include "GameplayTagStack.generated.h"

/**
 * 
 */


USTRUCT()
struct FGameplayTagStack
{
	GENERATED_BODY()

	FGameplayTagStack() {}
	FGameplayTagStack(FGameplayTag InTag, int32 InCount)
		:Tag(InTag)
		, Count(InCount)
	{
	}

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly)
	int32 Count = 0;

};

USTRUCT()
struct FGameplayTagStackContainer
{
	GENERATED_BODY()

	void AddTagStack(FGameplayTag Tag, int32 Count);
	void OverrideTagStack(FGameplayTag Tag, int32 Count);
	void RemoveTagStack(FGameplayTag Tag, int32 Count);
	bool ContainsTagStack(FGameplayTag Tag);
	int32 GetTagStackCount(FGameplayTag Tag);
	TArray<FGameplayTagStack> GetAllTagStack();


private:

	UPROPERTY()
	TArray<FGameplayTagStack> TagStackList;

	TMap<FGameplayTag, int32> TagStacks;
};