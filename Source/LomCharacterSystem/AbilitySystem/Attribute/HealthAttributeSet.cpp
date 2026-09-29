// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attribute/HealthAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

class FLifetimeProperty;

void UHealthAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	const FGameplayEffectContextHandle& effectContextHandle = Data.EffectSpec.GetEffectContext();
	//UE_LOG(LogTemp, Error, TEXT("damage instigator = %s"), *effectContextHandle.GetInstigator()->GetName());
	//UE_LOG(LogTemp, Error, TEXT("damage causer = %s"), *effectContextHandle.GetEffectCauser()->GetName());
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Snapshot before broadcasting: a death listener can apply another effect.
		const float OldHealth = healthBeforeChange;
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, FMath::Max(0.0f, GetHealthMax())));
		const float NewHealth = GetHealth();
		if (OldHealth > 0.0f && NewHealth <= 0.0f)
		{
			OnDead.Broadcast(effectContextHandle.GetInstigator(), &Data.EffectSpec, Data.EvaluatedData.Magnitude, OldHealth, NewHealth);

		}
		OnHealthChanged.Broadcast(effectContextHandle.GetInstigator(), &Data.EffectSpec, Data.EvaluatedData.Magnitude, OldHealth, NewHealth);

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

void UHealthAttributeSet::OnRep_Mana(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, Mana, value);
}

void UHealthAttributeSet::OnRep_ManaMax(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthAttributeSet, ManaMax, value);

}

void UHealthAttributeSet::PreAttributeChange(
	const FGameplayAttribute& Attribute,
	float& NewValue)
{
	

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, FMath::Max(0.0f, GetHealthMax()));
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(
			NewValue,
			0.0f,
			GetManaMax()
		);
	}
	Super::PreAttributeChange(Attribute, NewValue);
}

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	// Also covers health restored directly through the ASC, without an executed effect.
	if (Attribute == GetHealthAttribute() && OldValue <= 0.0f && NewValue > 0.0f)
	{
		OnReset.Broadcast();
	}
}

void UHealthAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, HealthMax, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthAttributeSet, ManaMax, COND_None, REPNOTIFY_Always);

	
}
