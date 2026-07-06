// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"


#include "GFAction_BindInputAssociations.generated.h"

/**
 * 
 */
class ULomInputConfig;
struct FComponentRequestHandle;

UCLASS(meta = (DisplayName = "BindInputAssociations"))
class LOMCHARACTERSYSTEM_API UGFAction_BindInputAssociations : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	void AddToWorld(const FWorldContext& worldContext, FGameFeatureStateChangeContext& context);
	void HandleStartGameInstance(UGameInstance* gameInstance, FGameFeatureStateChangeContext context);
	void HandleExtention(AActor* receiver, FName EventName);
	void BindAssociationse(APlayerController* receiver);


private:
	struct FStateChangeHandle
	{
		FDelegateHandle GameInstanceHandle;
		TSharedPtr<FComponentRequestHandle> RequestHandle;
	};
	TMap<FGameFeatureStateChangeContext, FStateChangeHandle> StateChangeHandles;

	UPROPERTY(EditAnywhere, Category = "Config")
	TArray<ULomInputConfig*> AssociationConfigs;
};
