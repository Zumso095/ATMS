// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CommonActivatableWidget.h"
#include "Widget/LomCommonActivatableWidget.h"

#include "LomGameUIPolicy.generated.h"

/**
 * 
 */
class ULomGameUIPrimaryLayout;

UCLASS(Blueprintable, Within = LomGameUIManagerSubsystem)
class LOMCHARACTERSYSTEM_API ULomGameUIPolicy : public UObject
{
	GENERATED_BODY()
public:
	virtual UWorld* GetWorld() const;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULomGameUIPrimaryLayout> DefaultLayoutClass;

public:
	void SetLayout(ULocalPlayer* newLocalPlayer);
	void SetLayout();
	void PushContentToLayer(FGameplayTag Layer, TSubclassOf<ULomCommonActivatableWidget> WidgetClass, bool Reset, bool LockedForPop);
	void PopFromLayer(FGameplayTag Layer, bool ForcePop);
	

private:
	TObjectPtr<ULomGameUIPrimaryLayout> currentLayout;

};
