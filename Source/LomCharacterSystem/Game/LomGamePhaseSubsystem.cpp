// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LomGamePhaseSubsystem.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "LomGameplayAbility_GamePhase.h"
#include "LomGameState.h"

void ULomGamePhaseSubsystem::StartPhase(TSubclassOf<ULomGameplayAbility_GamePhase> GamePhase)
{
	UWorld* world = GetWorld();
	if (world)
	{
		if (AGameStateBase* gameState = world->GetGameState())
		{
			if (ULomAbilitySystemComponent* asc = gameState->GetComponentByClass<ULomAbilitySystemComponent>()) {
				FGameplayAbilitySpec spec(GamePhase,1,0,this);
				asc->GiveAbilityAndActivateOnce(spec);
			}
		}
	}
}
