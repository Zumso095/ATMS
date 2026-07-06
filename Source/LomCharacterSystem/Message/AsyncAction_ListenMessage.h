// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTags.h"
#include "AsyncAction_ListenMessage.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageActionDelegate, UAsyncAction_ListenMessage*, Listener, FGameplayTag, Channel);

UCLASS(BlueprintType, meta = (HasDedicatedAsyncNode))
class LOMCHARACTERSYSTEM_API UAsyncAction_ListenMessage : public UCancellableAsyncAction
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnMessageActionDelegate MessageReceived;

	UFUNCTION(BlueprintCallable, Category = "MessageSystem", meta = (WorldContext = "WorldContextObject",BlueprintInternalUseOnly="true"))
	static UAsyncAction_ListenMessage* ListenMessageAsync(UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* Payload);

	virtual void Activate() override;

	UWorld* world;
	UObject* worldContextObject;
	FGameplayTag channel;
	UScriptStruct* Payload;
};
