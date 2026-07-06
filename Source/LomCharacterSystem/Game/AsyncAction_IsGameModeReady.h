// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_IsGameModeReady.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIsGameReadyDelegate);

UCLASS()
class LOMCHARACTERSYSTEM_API UAsyncAction_IsGameModeReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FIsGameReadyDelegate Ready;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAsyncAction_IsGameModeReady* IsGameModeReady(UObject* WorldContextObject);

	void Activate() override;

	void Finish();
private:
	UObject* worldContextObject;
};
