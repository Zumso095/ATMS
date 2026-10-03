#pragma once

#include "LomAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NPCBehaviorAttributeSet.generated.h"

UCLASS()
class LOMCHARACTERSYSTEM_API UNPCBehaviorAttributeSet : public ULomAttributeSet
{
	GENERATED_BODY()
	
public:	
	UNPCBehaviorAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Behavior", ReplicatedUsing = OnRep_WalkingSpeed)
	FGameplayAttributeData WalkingSpeed;
	ATTRIBUTE_ACCESSORS(UNPCBehaviorAttributeSet, WalkingSpeed);

	UPROPERTY(BlueprintReadOnly, Category = "Behavior", ReplicatedUsing = OnRep_DetectionDuration)
	FGameplayAttributeData DetectionDuration;
	ATTRIBUTE_ACCESSORS(UNPCBehaviorAttributeSet, DetectionDuration);

	UPROPERTY(BlueprintReadOnly, Category = "Behavior", ReplicatedUsing = OnRep_LoseDuration)
	FGameplayAttributeData LoseDuration;
	ATTRIBUTE_ACCESSORS(UNPCBehaviorAttributeSet, LoseDuration);
	
protected:
	UFUNCTION()
	virtual void OnRep_WalkingSpeed(const FGameplayAttributeData& OldWalkingSpeed);

	UFUNCTION()
	virtual void OnRep_DetectionDuration(const FGameplayAttributeData& OldDetectionDuration);

	UFUNCTION()
	virtual void OnRep_LoseDuration(const FGameplayAttributeData& OldLoseDuration);
};
