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

USTRUCT()
struct FLomAnchorWidgetRequest
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag AnchorTag;
	UPROPERTY()
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> Widget = nullptr;
	UPROPERTY()
	TWeakObjectPtr<ULomUIAnchorPoint> Anchor;
	UPROPERTY()
	TWeakObjectPtr<UObject> Owner;

	bool bHasOwner = false;
	bool bCreating = false;
};

UCLASS(BlueprintType, config=Game)
class LOMCHARACTERSYSTEM_API ULomGameUIManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public: 
	FOnLayoutAddedDelegate OnLayoutAdded;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
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

	// The handle remains usable even when the anchor has not been constructed yet.
	UFUNCTION(BlueprintCallable, Category = "UI", meta = (AdvancedDisplay = "Owner"))
	FGuid RequestContentToAnchor(FGameplayTag AnchorTag, TSubclassOf<UUserWidget> WidgetClass, UObject* Owner = nullptr);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveWidgetRequest(FGuid RequestHandle);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveWidgetsForOwner(UObject* Owner);

	UFUNCTION(BlueprintPure, Category = "UI")
	UUserWidget* GetWidgetForRequest(FGuid RequestHandle) const;

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
	
	UPROPERTY(Transient)
	TMap<FGameplayTag, TWeakObjectPtr<ULomUIAnchorPoint>> anchorPoints;
	UPROPERTY(Transient)
	TMap<FGuid, FLomAnchorWidgetRequest> widgetRequests;

	void TryCreateWidgetRequest(FGuid RequestHandle);
};
