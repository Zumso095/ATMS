// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/StaminaAttributeSet.h"
#include "Net/UnrealNetwork.h"


void UStaminaAttributeSet::OnRep_Stamina(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UStaminaAttributeSet, Stamina, value);

}

void UStaminaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UStaminaAttributeSet, Stamina, COND_None, REPNOTIFY_Always);



}

