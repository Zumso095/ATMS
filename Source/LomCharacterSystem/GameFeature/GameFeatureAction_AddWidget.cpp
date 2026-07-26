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
	//FChangeStateHandle& handle = StateChangeHandles.FindOrAdd(Context);
	UE_LOG(LogTemp, Error, TEXT("addWidget game feature activated internal message"));

	

	FChangeStateHandle& chsh = StateChangeHandles.FindOrAdd(Context);


	chsh.DelegateHandle	= FWorldDelegates::OnStartGameInstance.AddUObject(this, &UGameFeatureAction_AddWidget::HandleStartGameInstance, FGameFeatureStateChangeContext(Context));

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
	UE_LOG(LogTemp, Error, TEXT("gamefeature instance deactivating"));

	handle.RequestHandles.Empty();
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
		UE_LOG(LogTemp, Error, TEXT("I will add widgets"));
		

		UGameFrameworkComponentManager* componentManager = world->GetGameInstance()->GetSubsystem<UGameFrameworkComponentManager>();
		TSoftClassPtr<AActor> hud = ALomHUD::StaticClass();
		handle.RequestHandles.Add(componentManager->AddExtensionHandler(hud, UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleExtentionHandler, Context)));
		

		

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
	// I was forced to do this because seamles server does'n destroy old player controller but it doesn't have local player
	ALomHUD* lomHud = Cast<ALomHUD>(Actor);
	if (!lomHud)
	{
		return;
	}
	if (!lomHud->GetOwningPlayerController())
	{
		return;
	}
	if (!lomHud->GetOwningPlayerController()->GetLocalPlayer())
	{
		return;
	}
	/*UE_LOG(LogTemp, Error,
		TEXT("Actor=%s (%p) Event=%s"),
		*Actor->GetFullName(),
		Actor,
		*EventName.ToString());

	ALomHUD* HUD = Cast<ALomHUD>(Actor);

	if (HUD)
	{
		UE_LOG(LogTemp, Error,
			TEXT("HUD=%p PC=%p"),
			HUD,
			HUD->GetOwningPlayerController());
	}
	for (FConstPlayerControllerIterator It = Actor->GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		UE_LOG(LogTemp, Error,
			TEXT("PC=%p Local=%d ControllerId=%d HUD=%p"),
			PC,
			PC->IsLocalController(),
			PC->GetLocalPlayer() ? PC->GetLocalPlayer()->GetControllerId() : -1,
			PC->GetHUD());
		UE_LOG(LogTemp, Error,
			TEXT("PC=%p PendingKill=%d BeginDestroyed=%d"),
			PC,
			PC->IsPendingKillPending(),
			PC->HasAnyFlags(RF_BeginDestroyed));
	}*/

	ULomGameUIManagerSubsystem* UIManager = Actor->GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>();
	//UE_LOG(LogTemp, Error, TEXT("add widget = %i <==> event name = %s"),GetUniqueID(), *EventName.ToString());
	FChangeStateHandle& handle = StateChangeHandles.FindOrAdd(ChangeContext);

	if (EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved || EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved)
	{
		//UE_LOG(LogTemp, Error, TEXT(" event name = %s"), *EventName.ToString());
	//	FWorldDelegates::OnStartGameInstance.Remove(handle.DelegateHandle);
	//	handle.RequestHandles.Empty();

	}


	else if (EventName == UGameFrameworkComponentManager::NAME_GameActorReady  || EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded)
	{
		for (FLomGameUILayout& widget : Layouts)
		{
			//UIManager->GetGameUIPolicy()->PushContentToLayer(widget.LayerID, widget.LayoutClass, false,false);
			UIManager->PushContentToLayer(widget.LayerID, widget.LayoutClass, false, false);
			//UE_LOG(LogTemp, Error, TEXT("add layout "));

		}

		for (FLomGameUIWidget& widget : Widgets)
		{
			//UIManager->GetGameUIPolicy()->PushContentToLayer(widget.LayerID, widget.LayoutClass, false,false);
			UIManager->PushContentToAnchor(widget.SlotID, widget.WidgetClass);
			//UE_LOG(LogTemp, Error, TEXT("add widget "));

		}
	}
	//handle.RequestHandles.Empty();
}
