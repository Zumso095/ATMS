// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DeveloperSettings/LomLoadingScreenSettings.h"
#include "LomLoadingScreenSubsystem.generated.h"

/**
 * 
 */

class IInputProcessor;

UCLASS()
class LOMCHARACTERSYSTEM_API ULomLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:



	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override;

	UFUNCTION(BlueprintCallable)
	void PrepareLoadingWidget(FString MapName);

private:

	void HandlePreLoadMap(const FWorldContext& WorldContext, const FString& MapName);
	void HandlePostLoadMap(UWorld* World);
	void HandleSeamlessTravel(UWorld* World, const FString& MapName);

	void PreLoadMapInfo(FString MapName);

	void PrepareMoviePlayer();

	void UpdateLoadingScreen();

	void ShowLoadingScreen();

	void HideLoadingScreen();

	void RemoveWidgetFromViewport();

	bool ShouldShowLoadingScreen();

	bool CheckForAnyNeedToShowLoadingScreen();

	void ChangePerformanceSettings(bool bEnabingLoadingScreen);

	void StartBlockingInput();

	void StopBlockingInput();

	UUserWidget* UserWidget;

	TSharedPtr<SWidget> LoadingScreenWidget;

	TSharedPtr<IInputProcessor> InputPreProcessor;

	bool bCurrentlyInLoadMap = false;

	bool bStartupLoading = true;

	bool bCurrentlyShowingLoadingScreen = false;

	bool bSeamlessTravel = false;

	double TimeLoadingScreenShown = 0.0;
	double TimeLoadingScreenLastDismissed = -1.0;

	FString LoadingMapName = "";
};
