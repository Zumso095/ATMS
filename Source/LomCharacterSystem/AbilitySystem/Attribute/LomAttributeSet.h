#pragma once

#include "AttributeSet.h"
#include "LomAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class LOMCHARACTERSYSTEM_API ULomAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	virtual void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
};
