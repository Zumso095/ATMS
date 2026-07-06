// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomizerItemDefinition.h"
#include "CustomizerFragment_Stats.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMIZATIONSYSTEM_API UCustomizerFragment_Stats : public UCustomizerFragment
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = Equipment)
	TObjectPtr<AActor> myMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Equipment)
	int32 helpMe = 10;
};
