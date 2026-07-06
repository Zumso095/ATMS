// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget/LomCommonActivatableWidget.h"
#include "LomGameUIPrimaryLayout.generated.h"

/**
 * 
 */
//class UCommonActivatableWidget;
//class ULomCommonActivatableWidget;
class UCommonActivatableWidgetContainer;
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameUIPrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	//virtual UWidget* NativeGetDesiredFocusTarget() const;
	UFUNCTION(BlueprintCallable)
	void RegisterLayer(FGameplayTag layerTag, UCommonActivatableWidgetContainerBase* WidgetContainer);

	void PushContentToLayer(FGameplayTag layer, TSubclassOf<ULomCommonActivatableWidget> WidgetClass, bool Reset, bool LockedForPop);

	void PopFromLayer(FGameplayTag Layer, bool ForcePop);

	UFUNCTION()
	void SetInputMode(EInputMode InputMode, bool HideCursorDuringCapture, bool ShowCursor);

private:
	TMap<FGameplayTag,TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
};
