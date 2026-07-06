// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeature/GameFeatureAction_AddWidget.h"
#include "UI/LomGameUIManagerSubsystem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "UI/LomHUD.h"


void UGameFeatureAction_AddWidget::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// it doesn't work beacause enable activates in engine but works logging
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, TEXT("game feature"));
	//UE_LOG(LogTemp, Error, TEXT("Game feature activated"));
	/*FDelegateHandle* handle = StartGameInstanceDeligate.Find(Context);
	if (handle)
	{
		FWorldDelegates::OnStartGameInstance.Remove(*handle);
		UE_LOG(LogTemp, Error, TEXT("handle found and removed"));


	}*/

	StateChangeHandles.FindOrAdd(Context).DelegateHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddWidget::HandleStartGameInstance, FGameFeatureStateChangeContext(Context));

	for (const FWorldContext& worldContext : GEngine->GetWorldContexts())
	{
		//UE_LOG(LogTemp, Error, TEXT("contxt %s"), *worldContext.ContextHandle.ToString());

		if (Context.ShouldApplyToWorldContext(worldContext))
		{
			
			AddToWorld(worldContext,Context);
			//UE_LOG(LogTemp, Error, TEXT("addwidget == OnGameFeatureActivating pass the condition == %s"),*worldContext.ContextHandle.ToString());

		}
	}

}

void UGameFeatureAction_AddWidget::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FChangeStateHandle& handle = StateChangeHandles.FindOrAdd(Context);
	FWorldDelegates::OnStartGameInstance.Remove(handle.DelegateHandle);
		//UE_LOG(LogTemp, Error, TEXT("gamefeature instance deactivating"));

	handle.RequestHandles.Reset();
}

void UGameFeatureAction_AddWidget::AddToWorld(const FWorldContext& worldContext, FGameFeatureStateChangeContext& Context)
{
	FChangeStateHandle& handle = StateChangeHandles.FindOrAdd(Context);
	UWorld* world = worldContext.World();
	if (world && world->IsGameWorld() && !world->IsNetMode(ENetMode::NM_DedicatedServer))
	{
		//UE_LOG(LogTemp, Error, TEXT("this is a game world"));
		ULomGameUIManagerSubsystem* UIManager = world->GetSubsystem<ULomGameUIManagerSubsystem>();
		FString uipolicyName = UIManager->GetGameUIPolicy()->GetName();
		//const TCHAR charname = uipolicyName.GetCharArray()
		//UE_LOG(LogTemp, Error, TEXT("I will add widgets"));
		

		UGameFrameworkComponentManager* componentManager = world->GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>();
		TSoftClassPtr<AActor> hud = ALomHUD::StaticClass();
		handle.RequestHandles = componentManager->AddExtensionHandler(hud, UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleExtentionHandler, Context));
		

		

	}
}

void UGameFeatureAction_AddWidget::HandleStartGameInstance(UGameInstance* gameInstance, FGameFeatureStateChangeContext Context)
{
	//UE_LOG(LogTemp, Error, TEXT("add widget: HandleStartGameInstance"));
	
	FWorldContext* worldContext = gameInstance->GetWorldContext();
	if (Context.ShouldApplyToWorldContext(*worldContext)) 
	{
		//UE_LOG(LogTemp, Error, TEXT("add widget ShouldApplyToWorldContext"));

		AddToWorld(*worldContext,Context);
	}
	/*FDelegateHandle* handle = StartGameInstanceDeligate.Find(Context);
		if (handle)
		{
			FWorldDelegates::OnStartGameInstance.Remove(*handle);
		}*/
}

void UGameFeatureAction_AddWidget::HandleExtentionHandler(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	ULomGameUIManagerSubsystem* UIManager = Actor->GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>();
	//UE_LOG(LogTemp, Error, TEXT("add widget = %i <==> event name = %s"),GetUniqueID(), *EventName.ToString());

	if (EventName == UGameFrameworkComponentManager::NAME_GameActorReady || EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded)
	{
		for (FLomGameUILayout& widget : Layouts)
		{
			//UIManager->GetGameUIPolicy()->PushContentToLayer(widget.LayerID, widget.LayoutClass, false,false);
			UIManager->PushContentToLayer(widget.LayerID, widget.LayoutClass, false, false);
		}

		for (FLomGameUIWidget& widget : Widgets)
		{
			//UIManager->GetGameUIPolicy()->PushContentToLayer(widget.LayerID, widget.LayoutClass, false,false);
			UIManager->PushContentToAnchor(widget.SlotID, widget.WidgetClass);
		}
	}
}
