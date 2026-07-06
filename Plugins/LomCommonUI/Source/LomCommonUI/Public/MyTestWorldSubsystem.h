// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MyTestWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LOMCOMMONUI_API UMyTestWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int counter2 = 0l;
	
};
