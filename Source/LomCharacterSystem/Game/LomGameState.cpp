// Fill out your copyright notice in the Description page of Project Settings.


#include "LomGameState.h"
#include "Components/GameFrameworkComponentManager.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "LomGameModeManagerComponent.h"
#include <Net/UnrealNetwork.h>
//#include "AbilitySystemComponent.h"

ALomGameState::ALomGameState()
{

	abilitySystemComponent = CreateDefaultSubobject<ULomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	abilitySystemComponent->SetIsReplicated(true);

	gameModeManagerComponent = CreateDefaultSubobject<ULomGameModeManagerComponent>(TEXT("GameModeManagerComponent"));

}

void ALomGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ALomGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	abilitySystemComponent->InitAbilityActorInfo(this, this);
}

void ALomGameState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ALomGameState::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

int32 ALomGameState::GetTagStackCount(FGameplayTag Tag)
{
	return TagStackContainer.GetTagStackCount(Tag);
}

void ALomGameState::AddTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.AddTagStack(Tag, Count);
}

void ALomGameState::OverrideTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.OverrideTagStack(Tag, Count);
}

bool ALomGameState::ContainsTagStack(FGameplayTag Tag)
{
	return TagStackContainer.ContainsTagStack(Tag);
}

void ALomGameState::RemoveTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.RemoveTagStack(Tag, Count);
}

void ALomGameState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TagStackContainer);
}
