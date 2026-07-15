// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LomGameUIPolicy.h"
#include "GameplayTagContainer.h"
#include "CommonActivatableWidget.h"
#include "Widget/LomCommonActivatableWidget.h"
#include "Widget/LomUIAnchorPoint.h"
#include "LomGameUIManagerSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE(FOnLayoutAddedDelegate);

UCLASS(config=Game)
class LOMCHARACTERSYSTEM_API ULomGameUIManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public: 
	FOnLayoutAddedDelegate OnLayoutAdded;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	static ULomGameUIManagerSubsystem* Get() { return GEngine->GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>(); };
public:
	UPROPERTY(config)
	TSoftClassPtr<ULomGameUIPolicy> myConfig;
	UFUNCTION(BlueprintCallable)
	void PushContentToLayer(FGameplayTag Layer, TSubclassOf<ULomCommonActivatableWidget> WidgetClass, bool Reset = false, bool LockedForPop = false);

	UFUNCTION(BlueprintCallable)
	UUserWidget* PushContentToAnchor(FGameplayTag AnchorTag, TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable)
	void RemoveWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable)
	void PopFromLayer(FGameplayTag Layer, bool ForcePop = false);

	ULomGameUIPolicy* GetGameUIPolicy() { return currentGameUIPolicy; }

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void RegisterAnchorPoint(ULomUIAnchorPoint* AnchorPoint);
	void UnregisterAnchorPoint(ULomUIAnchorPoint* AnchorPoint);

	void LayoutAdded();
	void AddLayout();

	//void GetGameUIPolicy();

private:
	UPROPERTY()
	ULomGameUIPolicy* currentGameUIPolicy;
	
	TMap<FGameplayTag, ULomUIAnchorPoint*> anchorPoints;
	TMap<FGameplayTag, TSubclassOf<UUserWidget>> waitingWidgetsForLayout;
	TMap<UUserWidget*, ULomUIAnchorPoint*> addedWidgets;
};
