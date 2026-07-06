// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizerItemDefinition.h"

void UCustomizerAction::BeginPlay_Implementation(AActor* owner, UCustomizerItemDefinition* itemDefinition, class UCustomizerComponent* customizerComponent)
{
}

UCustomizerFragment* UCustomizerItemDefinition::FindFragmentByClass(TSubclassOf<UCustomizerFragment> fragmentClass)
{
	if (fragmentClass)
	{
		for (UCustomizerFragment* fragment : Fragments)
		{
			if (fragment && fragment->IsA(fragmentClass))
			{
				return fragment;
			}
		}
	}

	return nullptr;
}
