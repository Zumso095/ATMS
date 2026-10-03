#include "NPCPerceptionAttributeSet.h"

#include "Net/UnrealNetwork.h"

UNPCPerceptionAttributeSet::UNPCPerceptionAttributeSet()
{
	InitConeLength(0.0f);
	InitConeWidth(0.0f);
	InitNoiseZoneSize(0.0f);
	InitLookingDuration(0.0f);
}

void UNPCPerceptionAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UNPCPerceptionAttributeSet, ConeLength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNPCPerceptionAttributeSet, ConeWidth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNPCPerceptionAttributeSet, NoiseZoneSize, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNPCPerceptionAttributeSet, LookingDuration, COND_None, REPNOTIFY_Always);
}

void UNPCPerceptionAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UNPCPerceptionAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UNPCPerceptionAttributeSet::OnRep_ConeLength(const FGameplayAttributeData& OldConeLength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNPCPerceptionAttributeSet, ConeLength, OldConeLength);
}

void UNPCPerceptionAttributeSet::OnRep_ConeWidth(const FGameplayAttributeData& OldConeWidth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNPCPerceptionAttributeSet, ConeWidth, OldConeWidth);
}

void UNPCPerceptionAttributeSet::OnRep_NoiseZoneSize(const FGameplayAttributeData& OldNoiseZoneSize)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNPCPerceptionAttributeSet, NoiseZoneSize, OldNoiseZoneSize);
}

void UNPCPerceptionAttributeSet::OnRep_LookingDuration(const FGameplayAttributeData& OldLookingDuration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNPCPerceptionAttributeSet, LookingDuration, OldLookingDuration);
}