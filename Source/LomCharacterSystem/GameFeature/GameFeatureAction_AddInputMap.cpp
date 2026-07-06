// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeature/GameFeatureAction_AddInputMap.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Player/LomPlayerController.h"
#include "InputMappingContext.h"

void UGameFeatureAction_AddInputMap::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	//FGameFeatureStateChangeContext* ff = FGameFeatureStateChangeContext(Context);
	StateChangeHandles.FindOrAdd(Context).DelegateHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddInputMap::HandleOnGameInstanceStart, FGameFeatureStateChangeContext(Context));
		
	for (const FWorldContext& worldContex : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(worldContex))
		{
			AddToWorld(worldContex,Context);
		}
	}
}

void UGameFeatureAction_AddInputMap::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FStateChangeHandle& handle = StateChangeHandles.FindOrAdd(Context);
	FWorldDelegates::OnStartGameInstance.Remove(handle.DelegateHandle);
	handle.RequestHandle.Reset();
	
}

void UGameFeatureAction_AddInputMap::HandleOnGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext context)
{
	//if (GameInstance && GameInstance->GetWorld() && GameInstance->GetWorld()->IsGameWorld())
	//{
	//	
	//		
	//	//UE_LOG(LogTemp, Error, TEXT("world name %s"), *GameInstance->GetName());

	//}
	if (context.ShouldApplyToWorldContext(*GameInstance->GetWorldContext()))
	{
		AddToWorld(*GameInstance->GetWorldContext(), context);
	}
	

}

void UGameFeatureAction_AddInputMap::AddToWorld(const FWorldContext& WorldContext, FGameFeatureStateChangeContext& context)
{
	//UE_LOG(LogTemp, Error, TEXT("add to world"));
	FStateChangeHandle& handle = StateChangeHandles.FindOrAdd(context);
	UWorld* world = WorldContext.World();
	if (world && world->IsGameWorld() && !world->IsNetMode(NM_DedicatedServer))
	{
		UGameFrameworkComponentManager* componentManager = world->GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>();
		//componentManager->sen
		TSoftClassPtr<AActor> classToListen = APlayerController::StaticClass();
		handle.RequestHandle = componentManager->AddExtensionHandler(classToListen, UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &UGameFeatureAction_AddInputMap::HandleExtentionHandler));
	}
}



void UGameFeatureAction_AddInputMap::HandleExtentionHandler(AActor* Actor, FName EventName)
{
	if (Actor && EventName == ALomPlayerController::NAME_ReadyForInput || EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded)
	{
		APlayerController* playerController = Cast<APlayerController>(Actor);
		AddInputMapping(playerController);
	}
	//if (Actor &&  EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved)
	//{
	//	APlayerController* playerController = Cast<APlayerController>(Actor);
	//	RemoveInputMapping(playerController);
	//}
	//UE_LOG(LogTemp, Error, TEXT("event name == %s"), *EventName.ToString());

}

void UGameFeatureAction_AddInputMap::AddInputMapping(APlayerController* playerController)
{
	//APlayerController* pc = pawn->GetController<APlayerController>();
	if (playerController)
	{

		ULocalPlayer* lc = playerController->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(lc);
		for (UInputMappingContext* mapping : InputMappings)
		{
			
			enhancedInputSubsystem->AddMappingContext(mapping, 0);
			//UE_LOG(LogTemp, Error, TEXT("input mapping added == %s"), *mapping->GetName());

		}
	}
}

void UGameFeatureAction_AddInputMap::RemoveInputMapping(APlayerController* playerController)
{
	if (playerController)
	{

		ULocalPlayer* lc = playerController->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* enhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(lc);
		if (enhancedInputSubsystem)
		{
			for (UInputMappingContext* mapping : InputMappings)
			{
				enhancedInputSubsystem->RemoveMappingContext(mapping);
				UE_LOG(LogTemp, Error, TEXT("input mapping removed == %s"), *mapping->GetName());

			}
		}
		
	}
}
