// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameplayTagStack.h"

void FGameplayTagStackContainer::AddTagStack(FGameplayTag Tag, int32 Count)
{
	if (Tag.IsValid())
	{
		//TagStacks.Add(Tag, TagStacks.FindRef(Tag) + Count);

		for (int32 index = 0; index < TagStackList.Num(); index++)
		{
			if (TagStackList[index].Tag.MatchesTagExact(Tag))
			{
				TagStackList[index].Count += Count;
				return;
			}
		}
		TagStackList.Add(FGameplayTagStack(Tag, Count));

	}
}

void FGameplayTagStackContainer::OverrideTagStack(FGameplayTag Tag, int32 Count)
{
	if (Tag.IsValid())
	{
		//TagStacks.Add(Tag, TagStacks.FindRef(Tag) + Count);

		for (int32 index = 0; index < TagStackList.Num(); index++)
		{
			if (TagStackList[index].Tag.MatchesTagExact(Tag))
			{
				TagStackList[index].Count = Count;
				return;
			}
		}
		TagStackList.Add(FGameplayTagStack(Tag, Count));

	}
}

void FGameplayTagStackContainer::RemoveTagStack(FGameplayTag Tag, int32 Count)
{
	if (Tag.IsValid() && Count > 0)
	{
		for (int32 index = 0; index < TagStackList.Num(); index++)
		{
			if (TagStackList[index].Tag.MatchesTagExact(Tag))
			{
				TagStackList[index].Count -= Count;
				return;
			}
		}
		

	}
}

bool FGameplayTagStackContainer::ContainsTagStack(FGameplayTag Tag)
{
	for (int32 index = 0; index < TagStackList.Num(); index++)
	{
		if (TagStackList[index].Tag.MatchesTagExact(Tag))
		{
			
			return true;
		}
	}
	return false;
}

int32 FGameplayTagStackContainer::GetTagStackCount(FGameplayTag Tag)
{
	for (int32 index = 0; index < TagStackList.Num(); index++)
	{
		if (TagStackList[index].Tag.MatchesTagExact(Tag))
		{
			
			return TagStackList[index].Count;
		}
	}
	return 0;
}

TArray<FGameplayTagStack> FGameplayTagStackContainer::GetAllTagStack()
{
	return TagStackList;
}
