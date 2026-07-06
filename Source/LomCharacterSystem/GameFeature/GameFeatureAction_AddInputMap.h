// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include <EnhancedInputSubsystems.h>
#include <GameFeaturesSubsystem.h>

#include "GameFeatureAction_AddInputMap.generated.h"

struct FComponentRequestHandle;
/**
 * 
 */
UCLASS(meta = (DisplayName = "AddInputMapping"))
class LOMCHARACTERSYSTEM_API UGameFeatureAction_AddInputMap : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	void HandleOnGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext context);
	void AddToWorld(const FWorldContext& WorldContext, FGameFeatureStateChangeContext& context);
	void AddInputMapping(APlayerController* actor);
	void RemoveInputMapping(APlayerController* actor);
	void HandleExtentionHandler(AActor* Actor, FName EventName);

	UPROPERTY(EditAnywhere, Category = "InputMapping")
	TArray<UInputMappingContext*> InputMappings;

private:
	struct FStateChangeHandle
	{
		FDelegateHandle DelegateHandle;
		TSharedPtr<FComponentRequestHandle> RequestHandle;
	};
	TMap<FGameFeatureStateChangeContext, FStateChangeHandle> StateChangeHandles;
	
};