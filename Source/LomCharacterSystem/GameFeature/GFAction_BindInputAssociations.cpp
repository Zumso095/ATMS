// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeature/GFAction_BindInputAssociations.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/LomPlayerController.h"
#include "Input/LomInputComponent.h"

void UGFAction_BindInputAssociations::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	StateChangeHandles.FindOrAdd(Context).GameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGFAction_BindInputAssociations::HandleStartGameInstance, FGameFeatureStateChangeContext(Context));

	for (const FWorldContext& worldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(worldContext))
		{
			AddToWorld(worldContext,Context);
		}
	}
}

void UGFAction_BindInputAssociations::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FStateChangeHandle& handle = StateChangeHandles.FindOrAdd(Context);
	FWorldDelegates::OnStartGameInstance.Remove(handle.GameInstanceHandle);
	handle.RequestHandle.Reset();
}

void UGFAction_BindInputAssociations::HandleStartGameInstance(UGameInstance* gameInstance, FGameFeatureStateChangeContext context)
{
	if (context.ShouldApplyToWorldContext(*gameInstance->GetWorldContext()))
	{
		AddToWorld(*gameInstance->GetWorldContext(),context);
	}
}



void UGFAction_BindInputAssociations::AddToWorld(const FWorldContext& worldContext, FGameFeatureStateChangeContext& context)
{
	FStateChangeHandle& handle = StateChangeHandles.FindOrAdd(context);
	UWorld* world = worldContext.World();
	if (world && world->IsGameWorld() && !world->IsNetMode(NM_DedicatedServer))
	{
		UGameFrameworkComponentManager* componentManager = world->GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>();

		TSoftClassPtr<AActor> receiverClass = APlayerController::StaticClass();
		handle.RequestHandle = componentManager->AddExtensionHandler(receiverClass, UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &UGFAction_BindInputAssociations::HandleExtention));
	}

}

void UGFAction_BindInputAssociations::HandleExtention(AActor* receiver, FName EventName)
{
	if (receiver && EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == ALomPlayerController::NAME_ReadyForInput)
	{
		APlayerController* playerController = Cast<APlayerController>(receiver);
		BindAssociationse(playerController);
	}
}

void UGFAction_BindInputAssociations::BindAssociationse(APlayerController* playerController)
{
	if (playerController)
	{
		if (ULomInputComponent* inputComponent = playerController->FindComponentByClass<ULomInputComponent>())
		{
			for (ULomInputConfig* inputConfig : AssociationConfigs)
			{
				if (inputConfig)
				{
					inputComponent->BindInputConfig(inputConfig);
				}
			}
		}
	}
}
