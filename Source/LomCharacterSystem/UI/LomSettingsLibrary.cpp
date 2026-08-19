// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LomSettingsLibrary.h"

#include "Engine/GameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/LomGameSaveOffline.h"
#include "System/LomGameSaveSubsystem.h"

namespace
{
	ULomGameSaveSubsystem* GetSaveSubsystem(const UObject* WorldContextObject)
	{
		if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
		{
			return GameInstance->GetSubsystem<ULomGameSaveSubsystem>();
		}

		return nullptr;
	}
}

FString ULomSettingsLibrary::MakeSettingKey(FName SettingID)
{
	return FString::Printf(TEXT("Setting.%s"), *SettingID.ToString());
}

bool ULomSettingsLibrary::HasSavedSetting(const UObject* WorldContextObject, FName SettingID)
{
	ULomGameSaveSubsystem* SaveSubsystem = GetSaveSubsystem(WorldContextObject);

	return SaveSubsystem && SaveSubsystem->HasKey(MakeSettingKey(SettingID), ULomGameSaveOffline::StaticClass());
}

float ULomSettingsLibrary::ResolveSavedFloat(const UObject* WorldContextObject, FName SettingID, float DefaultValue)
{
	ULomGameSaveSubsystem* SaveSubsystem = GetSaveSubsystem(WorldContextObject);
	if (!SaveSubsystem)
	{
		return DefaultValue;
	}

	const FString Key = MakeSettingKey(SettingID);
	if (!SaveSubsystem->HasKey(Key, ULomGameSaveOffline::StaticClass()))
	{
		return DefaultValue;
	}

	return SaveSubsystem->GetFloat(Key, ULomGameSaveOffline::StaticClass());
}

int32 ULomSettingsLibrary::ResolveSavedListIndex(const UObject* WorldContextObject, FName SettingID, const TArray<FName>& Options, int32 DefaultIndex)
{
	ULomGameSaveSubsystem* SaveSubsystem = GetSaveSubsystem(WorldContextObject);
	if (!SaveSubsystem)
	{
		return DefaultIndex;
	}

	const FString Key = MakeSettingKey(SettingID);
	if (!SaveSubsystem->HasKey(Key, ULomGameSaveOffline::StaticClass()))
	{
		return DefaultIndex;
	}

	// The saved option may no longer exist in the list (for example a resolution from another monitor).
	const FName SavedValue(*SaveSubsystem->GetString(Key, ULomGameSaveOffline::StaticClass()));
	const int32 SavedIndex = Options.IndexOfByKey(SavedValue);

	return Options.IsValidIndex(SavedIndex) ? SavedIndex : DefaultIndex;
}

void ULomSettingsLibrary::SaveFloatSetting(const UObject* WorldContextObject, FName SettingID, float Value)
{
	if (ULomGameSaveSubsystem* SaveSubsystem = GetSaveSubsystem(WorldContextObject))
	{
		SaveSubsystem->SaveFloat(MakeSettingKey(SettingID), Value, ULomGameSaveOffline::StaticClass());
	}
}

void ULomSettingsLibrary::SaveNameSetting(const UObject* WorldContextObject, FName SettingID, FName Value)
{
	if (ULomGameSaveSubsystem* SaveSubsystem = GetSaveSubsystem(WorldContextObject))
	{
		SaveSubsystem->SaveString(MakeSettingKey(SettingID), Value.ToString(), ULomGameSaveOffline::StaticClass());
	}
}

void ULomSettingsLibrary::SavePendingSettings(const UObject* WorldContextObject, const TMap<FName, float>& FloatSettings, const TMap<FName, FName>& ListSettings)
{
	ULomGameSaveSubsystem* SaveSubsystem = GetSaveSubsystem(WorldContextObject);
	if (!SaveSubsystem)
	{
		return;
	}

	for (const TPair<FName, float>& Setting : FloatSettings)
	{
		SaveSubsystem->SaveFloat(MakeSettingKey(Setting.Key), Setting.Value, ULomGameSaveOffline::StaticClass());
	}

	for (const TPair<FName, FName>& Setting : ListSettings)
	{
		SaveSubsystem->SaveString(MakeSettingKey(Setting.Key), Setting.Value.ToString(), ULomGameSaveOffline::StaticClass());
	}
}

TArray<FName> ULomSettingsLibrary::GetAvailableResolutionOptions()
{
	// Returns false when the device cannot be queried; callers then keep their authored list.
	TArray<FIntPoint> Resolutions;
	if (!UKismetSystemLibrary::GetConvenientWindowedResolutions(Resolutions))
	{
		return TArray<FName>();
	}

	Resolutions.Sort([](const FIntPoint& A, const FIntPoint& B)
	{
		return A.X * A.Y < B.X * B.Y;
	});

	TArray<FName> Options;
	Options.Reserve(Resolutions.Num());
	for (const FIntPoint& Resolution : Resolutions)
	{
		Options.Add(FName(*FString::Printf(TEXT("%d x %d"), Resolution.X, Resolution.Y)));
	}

	return Options;
}

bool ULomSettingsLibrary::ParseResolutionOption(FName Option, FIntPoint& OutResolution)
{
	OutResolution = FIntPoint::ZeroValue;

	FString Width;
	FString Height;
	if (!Option.ToString().Split(TEXT("x"), &Width, &Height))
	{
		return false;
	}

	Width.TrimStartAndEndInline();
	Height.TrimStartAndEndInline();
	if (!Width.IsNumeric() || !Height.IsNumeric())
	{
		return false;
	}

	OutResolution = FIntPoint(FCString::Atoi(*Width), FCString::Atoi(*Height));

	return OutResolution.X > 0 && OutResolution.Y > 0;
}

FIntPoint ULomSettingsLibrary::ClampResolutionToDesktop(FIntPoint Resolution)
{
	if (!FSlateApplication::IsInitialized())
	{
		return Resolution;
	}

	FDisplayMetrics DisplayMetrics;
	FSlateApplication::Get().GetDisplayMetrics(DisplayMetrics);
	if (DisplayMetrics.PrimaryDisplayWidth <= 0 || DisplayMetrics.PrimaryDisplayHeight <= 0)
	{
		return Resolution;
	}

	return FIntPoint(
		FMath::Min(Resolution.X, DisplayMetrics.PrimaryDisplayWidth),
		FMath::Min(Resolution.Y, DisplayMetrics.PrimaryDisplayHeight));
}
