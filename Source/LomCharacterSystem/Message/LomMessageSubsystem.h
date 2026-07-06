// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTags.h"
#include "LomMessageSubsystem.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnMessageSend);
UCLASS()
class LOMCHARACTERSYSTEM_API ULomMessageSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	FOnMessageSend OnMessageSend;

	UFUNCTION(BlueprintCallable)
	void BroadcastMessageTemp(FGameplayTag Channel,UScriptStruct *Payload);

	/*UFUNCTION(BlueprintCallable)
	UScriptStruct* ListenMessageTemp(FGameplayTag Channel);*/

private:
	TMap<FGameplayTag, TArray<UObject*>> Listeners;
};
