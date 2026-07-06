// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeature/LomGameFeaturesProjectPolicies.h"
#include "GameFeatureData.h"
#include "GameFeatureAction_AddCuePath.h"
#include "GameplayCueManager.h"
#include "AbilitySystemGlobals.h"



void ULomGameFeaturesProjectPolicies::InitGameFeatureManager()
{
	AddCuePathsObserver = NewObject<UGFAction_AddCuePaths>();
	UGameFeaturesSubsystem::Get().AddObserver(AddCuePathsObserver);

	Super::InitGameFeatureManager();
}

void ULomGameFeaturesProjectPolicies::ShutdownGameFeatureManager()
{
	Super::ShutdownGameFeatureManager();
	UGameFeaturesSubsystem::Get().RemoveObserver(AddCuePathsObserver);
}

void UGFAction_AddCuePaths::OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
	
	const FString pluginRootPath = TEXT("/") + PluginName;
	//UE_LOG(LogTemp, Error, TEXT("%s"),*PluginURL);
	for (UGameFeatureAction* action : GameFeatureData->GetActions())
	{
		if (UGameFeatureAction_AddCuePath* addCuePathAction = Cast<UGameFeatureAction_AddCuePath>(action))
		{
			//UGameFeaturesSubsystem::Get().pat
			for (FDirectoryPath& path : addCuePathAction->GetCuePathsToAdd())
			{
				FString mutablePath = path.Path;
				UGameFeaturesSubsystem::Get().FixPluginPackagePath(mutablePath, pluginRootPath, false);
				UAbilitySystemGlobals::Get().GetGameplayCueManager()->AddGameplayCueNotifyPath(mutablePath, false);
				UE_LOG(LogGameFeatures, Display, TEXT("Found cue path to add: %s"),*mutablePath);

			}
			if (!addCuePathAction->GetCuePathsToAdd().IsEmpty())
			{
				UAbilitySystemGlobals::Get().GetGameplayCueManager()->InitializeRuntimeObjectLibrary();
			}
		}
	}
}

void UGFAction_AddCuePaths::OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL)
{
}
