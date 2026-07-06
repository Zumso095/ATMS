// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPoolable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UIPoolable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class OBJECTPOOLSYSTEM_API IIPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPoolable")
	bool IsActiveObject();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPoolable")
	void ActivateObject();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IPoolable")
	void DeactivateObject();


};
