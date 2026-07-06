// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "System/GameplayTagStack.h"


#include "LomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ALomPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	
	// TagStack
	UFUNCTION(BlueprintCallable)
	int32 GetTagStackCount(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	void AddTagStack(FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable)
	void OverrideTagStack(FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable)
	bool ContainsTagStack(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	void RemoveTagStack(FGameplayTag Tag, int32 Count);
	//End TagStack

private:

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TagStackContainer;
};
