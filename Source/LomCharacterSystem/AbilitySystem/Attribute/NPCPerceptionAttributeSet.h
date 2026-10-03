#pragma once

#include "LomAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "NPCPerceptionAttributeSet.generated.h"

UCLASS()
class LOMCHARACTERSYSTEM_API UNPCPerceptionAttributeSet : public ULomAttributeSet
{
	GENERATED_BODY()
	
public:
	UNPCPerceptionAttributeSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vision", ReplicatedUsing = OnRep_ConeLength)
	FGameplayAttributeData ConeLength;
	ATTRIBUTE_ACCESSORS(UNPCPerceptionAttributeSet, ConeLength);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Vision", ReplicatedUsing = OnRep_ConeWidth)
	FGameplayAttributeData ConeWidth;
	ATTRIBUTE_ACCESSORS(UNPCPerceptionAttributeSet, ConeWidth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Perception", ReplicatedUsing = OnRep_NoiseZoneSize)
	FGameplayAttributeData NoiseZoneSize;
	ATTRIBUTE_ACCESSORS(UNPCPerceptionAttributeSet, NoiseZoneSize);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Perception", ReplicatedUsing = OnRep_LookingDuration)
	FGameplayAttributeData LookingDuration;
	ATTRIBUTE_ACCESSORS(UNPCPerceptionAttributeSet, LookingDuration);
	
protected:
	UFUNCTION()
	virtual void OnRep_ConeLength(const FGameplayAttributeData& OldConeLength);

	UFUNCTION()
	virtual void OnRep_ConeWidth(const FGameplayAttributeData& OldConeWidth);

	UFUNCTION()
	virtual void OnRep_NoiseZoneSize(const FGameplayAttributeData& OldNoiseZoneSize);

	UFUNCTION()
	virtual void OnRep_LookingDuration(const FGameplayAttributeData& OldLookingDuration);
};
