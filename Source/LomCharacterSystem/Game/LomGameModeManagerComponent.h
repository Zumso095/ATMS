// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LomGameModeManagerComponent.generated.h"

/**
 * 
 */
namespace UE::GameFeatures { struct FResult; }

class ULomGameModeDefinition;
class UGameFeatureAction;
struct FGameFeatureDeactivatingContext;
struct FGameFeatureActivatingContext;

DECLARE_MULTICAST_DELEGATE(FOnGameModeDefeinitionReadyDelegate);

UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameModeManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:

	FOnGameModeDefeinitionReadyDelegate OnGameModeDefinitionReady;
	ULomGameModeManagerComponent(const FObjectInitializer& ObjectInitializer);
	virtual void Init();
	virtual void StartGameMode();
	virtual void SetGameModeDefinition(ULomGameModeDefinition* GameModeDefinition);
	virtual void EndGameMode();
	void ActivateActions(TArray<UGameFeatureAction*>& Actions, FGameFeatureActivatingContext& Context);
	void DeactivateActions(TArray<UGameFeatureAction*>& Actions, FGameFeatureDeactivatingContext& Context);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(ReplicatedUsing = OnRep_CurrentGameModeDefinition)
	ULomGameModeDefinition* CurrentGameModeDefinition;

	void PluginLoadCompleted(const UE::GameFeatures::FResult& Result);
	void GameModeLoadCompleted();

	//bool IsGameModeDefReady() { return isReady; }

	
	 bool IsGameModeDefReady;

private:
	UFUNCTION()
	void OnRep_CurrentGameModeDefinition();

	TArray<FString> PluginURLs;
	int32 pluginsToEnableAmount;
};
