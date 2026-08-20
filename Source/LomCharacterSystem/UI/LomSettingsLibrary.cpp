// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LomSettingsLibrary.h"

#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/GameUserSettings.h"
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

	struct FResolutionPreset
	{
		int32 Width;
		int32 Height;
		const TCHAR* AspectLabel;
	};

	/**
	 * Resolutions offered to the player, grouped by the aspect ratios desktop monitors actually use.
	 * This order is the order of the option list; entries the monitor cannot show are dropped.
	 */
	const FResolutionPreset ResolutionPresets[] =
	{
		{ 1024,  768, TEXT("4:3")   },
		{ 1280,  960, TEXT("4:3")   },
		{ 1400, 1050, TEXT("4:3")   },
		{ 1600, 1200, TEXT("4:3")   },
		{ 1920, 1440, TEXT("4:3")   },

		{ 1280, 1024, TEXT("5:4")   },

		{ 1440,  960, TEXT("3:2")   },
		{ 1920, 1280, TEXT("3:2")   },
		{ 2160, 1440, TEXT("3:2")   },
		{ 2880, 1920, TEXT("3:2")   },

		{ 1280,  800, TEXT("16:10") },
		{ 1440,  900, TEXT("16:10") },
		{ 1680, 1050, TEXT("16:10") },
		{ 1920, 1200, TEXT("16:10") },
		{ 2560, 1600, TEXT("16:10") },
		{ 3840, 2400, TEXT("16:10") },

		{ 1280,  720, TEXT("16:9")  },
		{ 1366,  768, TEXT("16:9")  },
		{ 1600,  900, TEXT("16:9")  },
		{ 1920, 1080, TEXT("16:9")  },
		{ 2560, 1440, TEXT("16:9")  },
		{ 3200, 1800, TEXT("16:9")  },
		{ 3840, 2160, TEXT("16:9")  },

		{ 2560, 1080, TEXT("21:9")  },
		{ 3440, 1440, TEXT("21:9")  },
		{ 3840, 1600, TEXT("21:9")  },
		{ 5120, 2160, TEXT("21:9")  },

		{ 3840, 1080, TEXT("32:9")  },
		{ 5120, 1440, TEXT("32:9")  },
	};

	/** Ratio label for a resolution that is not one of the presets, such as a laptop panel. */
	FString MakeAspectLabel(FIntPoint Resolution)
	{
		if (Resolution.X <= 0 || Resolution.Y <= 0)
		{
			return FString();
		}

		struct FKnownAspect
		{
			double Aspect;
			const TCHAR* Label;
		};

		static const FKnownAspect KnownAspects[] =
		{
			{ 4.0 / 3.0,   TEXT("4:3")   },
			{ 5.0 / 4.0,   TEXT("5:4")   },
			{ 3.0 / 2.0,   TEXT("3:2")   },
			{ 16.0 / 10.0, TEXT("16:10") },
			{ 16.0 / 9.0,  TEXT("16:9")  },
			{ 21.0 / 9.0,  TEXT("21:9")  },
			{ 32.0 / 9.0,  TEXT("32:9")  },
		};

		const double Aspect = static_cast<double>(Resolution.X) / static_cast<double>(Resolution.Y);
		for (const FKnownAspect& Known : KnownAspects)
		{
			// Marketing ratios are approximate (3440x1440 is sold as 21:9 but is 43:18), so match loosely.
			if (FMath::Abs(Aspect - Known.Aspect) <= Known.Aspect * 0.035)
			{
				return Known.Label;
			}
		}

		int32 Divisor = FMath::Max(Resolution.X, Resolution.Y);
		int32 Remainder = FMath::Min(Resolution.X, Resolution.Y);
		while (Remainder != 0)
		{
			const int32 Next = Divisor % Remainder;
			Divisor = Remainder;
			Remainder = Next;
		}

		return FString::Printf(TEXT("%d:%d"), Resolution.X / Divisor, Resolution.Y / Divisor);
	}

	FName MakeResolutionOption(FIntPoint Resolution, const TCHAR* AspectLabel)
	{
		return FName(*FString::Printf(TEXT("%d x %d (%s)"), Resolution.X, Resolution.Y, AspectLabel));
	}

	/** Largest resolution the primary monitor can display, independent of the mode it is in right now. */
	FIntPoint GetPrimaryMonitorMaxResolution()
	{
		if (!FSlateApplication::IsInitialized())
		{
			return FIntPoint::ZeroValue;
		}

		FDisplayMetrics DisplayMetrics;
		FSlateApplication::Get().GetDisplayMetrics(DisplayMetrics);

		// PrimaryDisplayWidth/Height describe the mode the desktop is in right now, which is the game's own
		// resolution while it runs in exclusive fullscreen. MonitorInfo keeps the panel's real maximum.
		FIntPoint Result(DisplayMetrics.PrimaryDisplayWidth, DisplayMetrics.PrimaryDisplayHeight);

		for (const FMonitorInfo& Monitor : DisplayMetrics.MonitorInfo)
		{
			if (!Monitor.bIsPrimary)
			{
				continue;
			}

			FIntPoint Native = Monitor.MaxResolution;
			if (Native.X <= 0 || Native.Y <= 0)
			{
				Native = FIntPoint(Monitor.NativeWidth, Monitor.NativeHeight);
			}

			if (Native.X > 0 && Native.Y > 0)
			{
				Result = Native;
			}

			break;
		}

		return Result;
	}

	/** Closest mode the display reports for exclusive fullscreen, which is the only thing it will accept. */
	FIntPoint SnapToSupportedFullscreenMode(FIntPoint Resolution)
	{
		TArray<FIntPoint> SupportedResolutions;
		if (!UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions) || SupportedResolutions.Num() == 0)
		{
			return Resolution;
		}

		if (SupportedResolutions.Contains(Resolution))
		{
			return Resolution;
		}

		FIntPoint Best = Resolution;
		int64 BestScore = TNumericLimits<int64>::Max();
		for (const FIntPoint& Supported : SupportedResolutions)
		{
			const int64 Score = FMath::Abs(static_cast<int64>(Supported.X) - Resolution.X) + FMath::Abs(static_cast<int64>(Supported.Y) - Resolution.Y);
			const bool bLargerAtSameScore = Score == BestScore && static_cast<int64>(Supported.X) * Supported.Y > static_cast<int64>(Best.X) * Best.Y;
			if (Score < BestScore || bLargerAtSameScore)
			{
				BestScore = Score;
				Best = Supported;
			}
		}

		return Best;
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
	const FIntPoint MaxResolution = GetPrimaryMonitorMaxResolution();

	TArray<FName> Options;
	Options.Reserve(UE_ARRAY_COUNT(ResolutionPresets) + 1);

	for (const FResolutionPreset& Preset : ResolutionPresets)
	{
		const FIntPoint Resolution(Preset.Width, Preset.Height);
		if (MaxResolution.X > 0 && MaxResolution.Y > 0 && (Resolution.X > MaxResolution.X || Resolution.Y > MaxResolution.Y))
		{
			continue;
		}

		Options.AddUnique(MakeResolutionOption(Resolution, Preset.AspectLabel));
	}

	// The monitor's own resolution is not always one of the presets (laptop panels especially),
	// but it is the one option the player must always be able to pick.
	if (MaxResolution.X > 0 && MaxResolution.Y > 0)
	{
		Options.AddUnique(MakeResolutionOption(MaxResolution, *MakeAspectLabel(MaxResolution)));
	}

	return Options;
}

bool ULomSettingsLibrary::ParseResolutionOption(FName Option, FIntPoint& OutResolution)
{
	OutResolution = FIntPoint::ZeroValue;

	// Accepts "1920 x 1080", the legacy "1920 x1080" spelling and the labelled "1920 x 1080 (16:9)"
	// form by simply taking the first two numbers of the option.
	const FString Text = Option.ToString();

	int32 Values[2] = { 0, 0 };
	int32 ValueCount = 0;
	int32 Cursor = 0;

	while (ValueCount < 2 && Cursor < Text.Len())
	{
		if (!FChar::IsDigit(Text[Cursor]))
		{
			++Cursor;
			continue;
		}

		int32 Value = 0;
		while (Cursor < Text.Len() && FChar::IsDigit(Text[Cursor]))
		{
			Value = Value * 10 + (Text[Cursor] - TEXT('0'));
			++Cursor;
		}

		Values[ValueCount++] = Value;
	}

	if (ValueCount < 2)
	{
		return false;
	}

	OutResolution = FIntPoint(Values[0], Values[1]);

	return OutResolution.X > 0 && OutResolution.Y > 0;
}

FIntPoint ULomSettingsLibrary::ClampResolutionToDesktop(FIntPoint Resolution)
{
	if (Resolution.X <= 0 || Resolution.Y <= 0)
	{
		return Resolution;
	}

	const UGameUserSettings* GameUserSettings = GEngine ? GEngine->GetGameUserSettings() : nullptr;
	const EWindowMode::Type WindowMode = GameUserSettings ? GameUserSettings->GetFullscreenMode() : EWindowMode::Windowed;

	// Never let the window grow past what the monitor can show, and keep the aspect ratio while doing so.
	const FIntPoint MaxResolution = GetPrimaryMonitorMaxResolution();
	FIntPoint Result = Resolution;
	if (MaxResolution.X > 0 && MaxResolution.Y > 0)
	{
		const double Scale = FMath::Min3(1.0,
			static_cast<double>(MaxResolution.X) / static_cast<double>(Resolution.X),
			static_cast<double>(MaxResolution.Y) / static_cast<double>(Resolution.Y));

		if (Scale < 1.0)
		{
			Result = FIntPoint(FMath::RoundToInt(Resolution.X * Scale), FMath::RoundToInt(Resolution.Y * Scale));
		}
	}

	// Exclusive fullscreen only accepts modes the display reports. Asking for anything else makes the
	// driver fall back to a mode of its own choosing, which is why fullscreen showed a resolution the
	// player never picked.
	if (WindowMode == EWindowMode::Fullscreen)
	{
		Result = SnapToSupportedFullscreenMode(Result);
	}

	return Result;
}
