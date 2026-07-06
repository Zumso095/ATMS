// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/HealthAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

class FLifetimeProperty;

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayEffectContextHandle& effectContextHandle = Data.EffectSpec.GetEffectContext();
	//UE_LOG(LogTemp, Error, TEXT("damage instigator = %s"), *effectContextHandle.GetInstigator()->GetName());
	//UE_LOG(LogTemp, Error, TEXT("damage causer = %s"), *effectContextHandle.GetEffectCauser()->GetName());
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (GetHealth() <= 0)
		{
			OnDead.Broadcast(effectContextHandle.GetInstigator(), &Data.EffectSpec, Data.EvaluatedData.Magnitude, healthBeforeChange, GetHealth());

		}
		if ((Data.EvaluatedData.Magnitude + GetHealth()) > GetHealthMax())
		{
			SetHealth(GetHealthMax());
			//UE_LOG(LogTemp, Error, TEXT("health = %f"), Data.EvaluatedData.Magnitude);

		}
		OnHealthChanged.Broadcast(effectContextHandle.GetInstigator(), &Data.EffectSpec, Data.EvaluatedData.Magnitude,healthBeforeChange,GetHealth());

	}
	if (GetHealth() >= 100)
	{
		OnReset.Broadcast();
	}


}

bool UHealthAttributeSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	healthBeforeChange = GetHealth();

	//const FGameplayEffectContextHandle& effectContextHandle = Data.EffectSpec.GetEffectContext();


	//if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	//{
	//	
	//	if (Data.EvaluatedData.Magnitude > GetHealthMax())
	//	{
	//		//OnDead.Broadcast(effectContextHandle.GetInstigator(), &Data.EffectSpec, Data.EvaluatedData.Magnitude, healthBeforeChange, GetHealth());
	//		//FMath::Clamp(Data.EvaluatedData.Magnitude, 0, GetHealthMax());
	//	}

	//}
	return true;
}

void UHealthAttributeSet::OnRep_Health(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Health, value);
}

void UHealthAttributeSet::OnRep_HealthMax(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, HealthMax, value);

}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);

	
}