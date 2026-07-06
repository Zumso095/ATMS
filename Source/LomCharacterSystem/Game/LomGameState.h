// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AbilitySystemInterface.h"
#include <System/GameplayTagStack.h>

#include "LomGameState.generated.h"

/**
 * 
 */
//
class UAbilitySystemComponent;
class ULomGameModeManagerComponent;

UCLASS()
class LOMCHARACTERSYSTEM_API ALomGameState : public AGameStateBase,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ALomGameState();
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return abilitySystemComponent; };

	ULomGameModeManagerComponent* GetGameModeManager() { return gameModeManagerComponent; }

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

	//virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:
	UAbilitySystemComponent* abilitySystemComponent;
	ULomGameModeManagerComponent* gameModeManagerComponent;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TagStackContainer;
};
