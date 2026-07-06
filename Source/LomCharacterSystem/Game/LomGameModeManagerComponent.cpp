// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LomGameModeManagerComponent.h"
#include "LomWorldSettings.h"
#include "LomGameMode.h"
#include "LomGameModeActionSet.h"
#include "LomGameModeDefinition.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include <Net/UnrealNetwork.h>


//bool ULomGameModeManagerComponent::IsGameModeDefReady = false;

ULomGameModeManagerComponent::ULomGameModeManagerComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	pluginsToEnableAmount = 0;
	IsGameModeDefReady = false;
	SetIsReplicatedByDefault(true);
}

void ULomGameModeManagerComponent::Init()
{
	if (ALomGameMode* gameMode = GetGameMode<ALomGameMode>())
	{
		ALomWorldSettings* worldSettigns = Cast<ALomWorldSettings>(gameMode->GetWorldSettings());
		if (!worldSettigns)
		{
			return;

		}

		if (worldSettigns->GetDefaultGameModeDefinition())
		{
			SetGameModeDefinition(worldSettigns->GetDefaultGameModeDefinition());
		}


	}
}

void ULomGameModeManagerComponent::StartGameMode()
{
	if (CurrentGameModeDefinition)
	{
		FGameFeatureActivatingContext Context;

		const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (ExistingWorldContext)
		{
			Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			//UE_LOG(LogTemp, Error, TEXT("world context"));

		}
		
		ActivateActions(CurrentGameModeDefinition->GameFeatureActions, Context);
		for (ULomGameModeActionSet* actionSet : CurrentGameModeDefinition->GameFeatureActionSets)
		{
			ActivateActions(actionSet->GameFeatureActions, Context);

		}

		PluginURLs.Reset();
		
		auto CollectPluginsToEnable = [this](const TArray<FString>& Plugins)
			{
				for (FString pluginName : Plugins)
				{
					FString pluginURL;
					bool pluginExists = UGameFeaturesSubsystem::Get().GetPluginURLByName(pluginName, pluginURL);
					if (pluginExists)
					{
						//UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(pluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ULomGameModeManagerComponent::PluginLoadCompleted));
						PluginURLs.Add(pluginURL);

					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("can't load plugin with name %s"),*pluginName);

					}

				}
				
			};
		CollectPluginsToEnable(CurrentGameModeDefinition->PluginsToEnable);
		for (ULomGameModeActionSet* actionSet : CurrentGameModeDefinition->GameFeatureActionSets)
		{
			if (actionSet)
			{
				CollectPluginsToEnable(actionSet->PluginsToEnable);
			}
		}
		
		pluginsToEnableAmount = PluginURLs.Num();
		if (PluginURLs.Num() > 0)
		{
			for (FString pluginURL : PluginURLs)
			{
				UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(pluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ULomGameModeManagerComponent::PluginLoadCompleted));
			}
		}
		else
		{
			GameModeLoadCompleted();
		}
		

		
	}
	/*UGameFeaturesSubsystem* gameFeatureSubsytem = GEngine->GetEngineSubsystem<UGameFeaturesSubsystem>();
	if (gameFeatureSubsytem)
	{
		UE_LOG(LogTemp, Error, TEXT("game feature subsystem exists"));
		gameFeatureSubsytem->LoadAndActivateGameFeaturePlugin(FName());

	}*/
	

}
void ULomGameModeManagerComponent::PluginLoadCompleted(const UE::GameFeatures::FResult& Result)
{
	pluginsToEnableAmount--;
	//UE_LOG(LogTemp, Error, TEXT("game feature plugin loaded %i"),pluginsToEnableAmount);
	if (pluginsToEnableAmount == 0)
	{
		GameModeLoadCompleted();
	}

}

void ULomGameModeManagerComponent::GameModeLoadCompleted()
{
	IsGameModeDefReady = true;
	OnGameModeDefinitionReady.Broadcast();
}

void ULomGameModeManagerComponent::SetGameModeDefinition(ULomGameModeDefinition* GameModeDefinition)
{
	if (GameModeDefinition)
	{
		CurrentGameModeDefinition = GameModeDefinition;

		
		StartGameMode();
	}
}
void ULomGameModeManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	EndGameMode();

}
void ULomGameModeManagerComponent::EndGameMode()
{
	if(CurrentGameModeDefinition)
	{
		FGameFeatureDeactivatingContext Context(TEXT(""), [](FStringView pauserTag) {});
		const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (ExistingWorldContext)
		{
			Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
		}
		DeactivateActions(CurrentGameModeDefinition->GameFeatureActions,Context);
		for (ULomGameModeActionSet* actionSet : CurrentGameModeDefinition->GameFeatureActionSets)
		{
			DeactivateActions(actionSet->GameFeatureActions,Context);
		}

		for (FString& pluginURL : PluginURLs)
		{
			UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(pluginURL);
		}

	}
}

void ULomGameModeManagerComponent::ActivateActions(TArray<UGameFeatureAction*>& Actions, FGameFeatureActivatingContext& Context)
{
	
	for (UGameFeatureAction* action : Actions)
	{

		if (action)
		{
			//UE_LOG(LogTemp, Error, TEXT("action"));

			action->OnGameFeatureRegistering();
			action->OnGameFeatureLoading();
			action->OnGameFeatureActivating(Context);
		}
	}
}

void ULomGameModeManagerComponent::DeactivateActions(TArray<UGameFeatureAction*>& Actions, FGameFeatureDeactivatingContext& Context)
{
	//UE_LOG(LogTemp, Error, TEXT("deactivation actions"));

	for (UGameFeatureAction* action : Actions)
	{

		if (action)
		{
			//UE_LOG(LogTemp, Error, TEXT("action deactivated == %s"),*action->GetName());
			action->OnGameFeatureDeactivating(Context);
			action->OnGameFeatureUnregistering();
		}
	}

}





void ULomGameModeManagerComponent::OnRep_CurrentGameModeDefinition()
{
	StartGameMode();
}

void ULomGameModeManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentGameModeDefinition);
}
