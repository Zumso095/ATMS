// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LomSettingsTypes.generated.h"

/**
 * Type of the widget row used to edit a setting.
 */
UENUM(BlueprintType)
enum class ESettingRowType : uint8
{
	Slider			UMETA(DisplayName = "Slider"),
	DropdownList	UMETA(DisplayName = "Dropdown List"),
	CycleList		UMETA(DisplayName = "Cycle List")
};

/**
 * Definition of a single setting row: its id, label and the data its row widget needs.
 * Slider and list fields hide each other in the details panel based on RowType.
 */
USTRUCT(BlueprintType)
struct FSettingsDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FName SettingsID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText SettingName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	ESettingRowType RowType = ESettingRowType::Slider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "RowType == ESettingRowType::Slider", EditConditionHides))
	float MinSliderValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "RowType == ESettingRowType::Slider", EditConditionHides))
	float MaxSliderValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "RowType == ESettingRowType::Slider", EditConditionHides))
	float InitialSliderValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "RowType != ESettingRowType::Slider", EditConditionHides))
	TArray<FName> List;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (EditCondition = "RowType != ESettingRowType::Slider", EditConditionHides))
	int32 InitialListIndex = 0;
};

/**
 * A settings tab: a display name and the settings shown on it.
 */
USTRUCT(BlueprintType)
struct FSettingTab
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FText TabName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TArray<FSettingsDefinition> SettingsList;
};
