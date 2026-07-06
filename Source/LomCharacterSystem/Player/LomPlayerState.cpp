// Fill out your copyright notice in the Description page of Project Settings.


#include "LomPlayerState.h"
#include "Components/GameFrameworkComponentManager.h"
#include <Net\UnrealNetwork.h>

void ALomPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
	
}

void ALomPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ALomPlayerState::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void ALomPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TagStackContainer);
}



int32 ALomPlayerState::GetTagStackCount(FGameplayTag Tag)
{
	return TagStackContainer.GetTagStackCount(Tag);
}

void ALomPlayerState::AddTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.AddTagStack(Tag, Count);
}

void ALomPlayerState::OverrideTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.OverrideTagStack(Tag, Count);
}

bool ALomPlayerState::ContainsTagStack(FGameplayTag Tag)
{
	return TagStackContainer.ContainsTagStack(Tag);
}

void ALomPlayerState::RemoveTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.RemoveTagStack(Tag, Count);
}