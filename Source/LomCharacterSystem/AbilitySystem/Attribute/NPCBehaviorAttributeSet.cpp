#include "NPCBehaviorAttributeSet.h"

#include "Net/UnrealNetwork.h"

UNPCBehaviorAttributeSet::UNPCBehaviorAttributeSet()
{
	InitWalkingSpeed(0.0f);
	InitDetectionDuration(0.0f);
	InitLoseDuration(0.0f);
}

void UNPCBehaviorAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UNPCBehaviorAttributeSet, WalkingSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNPCBehaviorAttributeSet, DetectionDuration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UNPCBehaviorAttributeSet, LoseDuration, COND_None, REPNOTIFY_Always);
}

void UNPCBehaviorAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	ClampAttribute(Attribute, NewValue);
}

void UNPCBehaviorAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	ClampAttribute(Attribute, NewValue);
}

void UNPCBehaviorAttributeSet::OnRep_WalkingSpeed(const FGameplayAttributeData& OldWalkingSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNPCBehaviorAttributeSet, WalkingSpeed, OldWalkingSpeed);
}

void UNPCBehaviorAttributeSet::OnRep_DetectionDuration(const FGameplayAttributeData& OldDetectionDuration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNPCBehaviorAttributeSet, DetectionDuration, OldDetectionDuration);
}

void UNPCBehaviorAttributeSet::OnRep_LoseDuration(const FGameplayAttributeData& OldLoseDuration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UNPCBehaviorAttributeSet, LoseDuration, OldLoseDuration);
}
