// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/SpeedAttributeSet.h"
#include <Net/UnrealNetwork.h>

void USpeedAttributeSet::OnRep_Speed(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(USpeedAttributeSet, Speed, value);

}

void USpeedAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(USpeedAttributeSet, Speed, COND_None, REPNOTIFY_Always);
	


}