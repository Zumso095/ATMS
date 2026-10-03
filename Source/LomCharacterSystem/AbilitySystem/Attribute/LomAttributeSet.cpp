#include "AbilitySystem/Attribute/LomAttributeSet.h"

void ULomAttributeSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	NewValue = FMath::Clamp(NewValue, 0.0f, TNumericLimits<float>::Max());
}
