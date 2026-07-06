// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/LomCommonActivatableWidget.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameFeatureAction_AddWidget.generated.h"

/**
 * 
 */
struct FComponentRequestHandle;

USTRUCT()
struct FLomGameUILayout
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<ULomCommonActivatableWidget> LayoutClass;

	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTag LayerID;
};

USTRUCT()
struct FLomGameUIWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTag SlotID;
};

UCLASS( meta = (DisplayName = "Add Widget"))
class LOMCHARACTERSYSTEM_API UGameFeatureAction_AddWidget : public UGameFeatureAction
{
	GENERATED_BODY()
public:
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

public:
	UPROPERTY(EditAnywhere, Category=UI, meta = (TitleProperty = "{LayerID} -> {LayoutClass}"))
	TArray<FLomGameUILayout> Layouts;

	UPROPERTY(EditAnywhere, Category = UI, meta = (TitleProperty = "{SlotID} -> {WidgetClass}"))
	TArray<FLomGameUIWidget> Widgets;

public:
	void AddToWorld(const FWorldContext& worldContext, FGameFeatureStateChangeContext& Context);
	void HandleStartGameInstance(UGameInstance* gameInstance, FGameFeatureStateChangeContext Context);
	void HandleExtentionHandler(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

private:
	struct FChangeStateHandle
	{
		FDelegateHandle DelegateHandle;
		TSharedPtr<FComponentRequestHandle> RequestHandles;

	};
	TMap<FGameFeatureStateChangeContext, FChangeStateHandle> StateChangeHandles;
	
};
