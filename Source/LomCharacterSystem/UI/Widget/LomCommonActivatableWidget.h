// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "LomCommonActivatableWidget.generated.h"

/**
 * 
 */
UENUM()
enum class EInputMode: uint8
{
	UIOnly,
	GameOnly,
	GameAndUI
};

UCLASS()
class LOMCHARACTERSYSTEM_API ULomCommonActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Lock")
	bool LockedForPop = false;
	
	UPROPERTY(EditAnywhere, Category = "InputMode")
	EInputMode InputMode = EInputMode::GameAndUI;

	UPROPERTY(EditAnywhere, Category = "InputMode")
	bool ShowCursor = false;

	UPROPERTY(EditAnywhere, Category = "InputMode")
	bool HideCursorDuringCapture = false;
};
