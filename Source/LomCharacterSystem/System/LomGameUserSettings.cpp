// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LomGameUserSettings.h"
#include "HAL/IConsoleManager.h"

#include "FMODStudioModule.h"
#include "fmod_studio.hpp"


namespace
{
	FMOD::Studio::System* GetRuntimeStudioSystem()
	{
		if (!IFMODStudioModule::IsAvailable())
		{
			return nullptr;
		}

		return IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
	}

    void ApplyBusVolume(FMOD::Studio::System* StudioSystem, const TCHAR* BusPath, float Volume)
    {
        if (!StudioSystem)
        {
            return;
        }

        FMOD::Studio::Bus* Bus = nullptr;

        const FMOD_RESULT GetResult = StudioSystem->getBus(TCHAR_TO_UTF8(BusPath), &Bus);

        if (GetResult != FMOD_OK || !Bus)
        {
            UE_LOG(LogTemp, Warning, TEXT("FMOD: Failed get Bus '%s'. Result: %d"), BusPath, static_cast<int32>(GetResult));
            return;
        }

        const FMOD_RESULT VolumeResult = Bus->setVolume(FMath::Clamp(Volume, 0.0f, 1.0f));
        if (VolumeResult != FMOD_OK)
        {
            UE_LOG(LogTemp, Warning, TEXT("FMOD: Failed set volume for Bus '%s'. Result: %d"), BusPath, static_cast<int32>(VolumeResult));
        }
    }

    void ApplySingleBusVolume(const TCHAR* BusPath, float Volume)
    {
        FMOD::Studio::System* StudioSystem = GetRuntimeStudioSystem();

        if (!StudioSystem)
        {
            return;
        }

        ApplyBusVolume(StudioSystem, BusPath, Volume);
    }
}

void ULomGameUserSettings::ApplyNonResolutionSettings()
{
	Super::ApplyNonResolutionSettings();
	ApplyMotionBlurSettings();
    ApplyAidioSettings();


}

void ULomGameUserSettings::SetMotionBlurEnabled(bool bEnabled)
{
	bMotionBlurEnabled = bEnabled;
}

void ULomGameUserSettings::SetMasterVolume(float Volume)
{
	MasterVolume = Volume;

    ApplySingleBusVolume(TEXT("bus:/MASTER"), MasterVolume);
}

void ULomGameUserSettings::SetMusicVolumeVolume(float Volume)
{
	MusicVolume = Volume;

    ApplySingleBusVolume(TEXT("bus:/MUSIC"), MusicVolume);
}

void ULomGameUserSettings::SetSFXVolume(float Volume)
{
	SFXVolume = Volume;

    ApplySingleBusVolume(TEXT("bus:/SFX"), SFXVolume);
}

void ULomGameUserSettings::SetAmbienceVolume(float Volume)
{
	AmbienceVolume = Volume;

    ApplySingleBusVolume(TEXT("bus:/ANBIENCE"), AmbienceVolume);
}

void ULomGameUserSettings::SetUIVolume(float Volume)
{
	UIVolume = Volume;

    ApplySingleBusVolume(TEXT("bus:/UI"), UIVolume);
}

void ULomGameUserSettings::ApplyAidioSettings()
{
    FMOD::Studio::System* StudioSystem = GetRuntimeStudioSystem();

    if (!StudioSystem)
    {
        return;
    }

    ApplyBusVolume(StudioSystem, TEXT("bus:/MASTER"), MasterVolume);
    ApplyBusVolume(StudioSystem, TEXT("bus:/MUSIC"), MusicVolume);
    ApplyBusVolume(StudioSystem, TEXT("bus:/SFX"), SFXVolume);
    ApplyBusVolume(StudioSystem, TEXT("bus:/AMBIENCE"), AmbienceVolume);
    ApplyBusVolume(StudioSystem, TEXT("bus:/UI"), UIVolume);

}

void ULomGameUserSettings::ApplyMotionBlurSettings()
{
	if (IConsoleVariable* MotionBlurQuality = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MotionBlurQuality")))
	{
		MotionBlurQuality->Set(bMotionBlurEnabled ? 3 : 0, ECVF_SetByGameSetting);
	}
}

