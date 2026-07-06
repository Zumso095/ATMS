// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizerComponent.h"
#include "CustomizerItemDefinition.h"

// Sets default values for this component's properties
UCustomizerComponent::UCustomizerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCustomizerComponent::BeginPlay()
{
	Super::BeginPlay();
	for (UCustomizerItemDefinition* itemDef : InitialCustomizerItemDefinition)
	{
		AddItemDefinition(itemDef);
	}
	// ...
	
}


// Called every frame
void UCustomizerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCustomizerComponent::AddItemDefinition(UCustomizerItemDefinition* definition)
{
	if (definition)
	{

		ItemDefinitionList.Add(definition);
		for (auto def : definition->Actions)
		{
			if (def)
			{
				def->BeginPlay(GetOwner(), definition, this);
			}

		}
		
	}
}

void UCustomizerComponent::AddAction(UCustomizerAction* action)
{
	if (action)
	{
		action->BeginPlay(GetOwner(), nullptr, this);
	}
}

void UCustomizerComponent::ClearCache(FName key)
{
	TArray<UActorComponent*>* componentArray = CacheComponent.Find(key);
	if (componentArray)
	{
		for (UActorComponent* ac : *componentArray)
		{
			ac->DestroyComponent();
		}
		componentArray->Empty();
	}
}

void UCustomizerComponent::AddCache(FName key, UActorComponent* cachedComponent)
{
	if (cachedComponent)
	{
		TArray<UActorComponent*>* componentArray = CacheComponent.Find(key);

		if (componentArray)
		{
			componentArray->Add(cachedComponent);
			return;
		}
		TArray<UActorComponent*> newArray;
		newArray.Add(cachedComponent);
		CacheComponent.Add(key, newArray);
	}
}

TArray<UActorComponent*> UCustomizerComponent::GetCache(FName key)
{
	if (CacheComponent.Find(key))
	{
		return *CacheComponent.Find(key);
	}
	return TArray<UActorComponent*>();
}

