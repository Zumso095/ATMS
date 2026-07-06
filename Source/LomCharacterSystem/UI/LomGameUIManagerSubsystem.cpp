// Fill out your copyright notice in the Description page of Project Settings.


#include "LomGameUIManagerSubsystem.h"
#include "LomGameUISettings.h"

DECLARE_LOG_CATEGORY_CLASS(LogGameUI, All, All);

void ULomGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	/*if (GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		return;
	}*/

	const ULomGameUISettings* settings = GetDefault<ULomGameUISettings>();
	TSubclassOf<ULomGameUIPolicy> configClass = settings->GameUIPolicy.TryLoadClass<ULomGameUIPolicy>();
	

	if (configClass) {
		//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, GetWorld()->GetName());

		ULomGameUIPolicy* configObject = NewObject<ULomGameUIPolicy>(this,configClass);
		if (configObject) {
			//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red,configObject->myname);
			currentGameUIPolicy = configObject;
			//currentGameUIPolicy->SetLayout();
			
		}
	}
}

void ULomGameUIManagerSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void ULomGameUIManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	currentGameUIPolicy->SetLayout();
}

void ULomGameUIManagerSubsystem::PushContentToLayer(FGameplayTag layer, TSubclassOf<ULomCommonActivatableWidget> widgetClass, bool reset, bool lockedForPop)
{
	currentGameUIPolicy->PushContentToLayer(layer, widgetClass, reset, lockedForPop);
	LayoutAdded();
}

UUserWidget* ULomGameUIManagerSubsystem::PushContentToAnchor(FGameplayTag AnchorTag, TSubclassOf<UUserWidget> WidgetClass)
{
	if (anchorPoints.Find(AnchorTag))
	{
		if (ULomUIAnchorPoint* anchor = *anchorPoints.Find(AnchorTag))
		{
			UUserWidget* widget = anchor->AddChild(WidgetClass);
			addedWidgets.Add(widget, anchor);
			//UE_LOG(LogGameUI, Error, TEXT("widget added == %s "), *widget->GetName());

			return widget;
		}
	}
	waitingWidgetsForLayout.Add(AnchorTag, WidgetClass);
	
	return nullptr;
}

void ULomGameUIManagerSubsystem::PopFromLayer(FGameplayTag Layer, bool forcePop)
{
	currentGameUIPolicy->PopFromLayer(Layer,forcePop);
}

bool ULomGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;

	}
	UWorld* World = Cast<UWorld>(Outer);
	bool gameInstanceExist = false;
	//gameInstanceExist = !World->GetGameInstance()->IsDedicatedServerInstance();
	if (World->GetGameInstance())
	{
		gameInstanceExist = !World->GetGameInstance()->IsDedicatedServerInstance();
		if (!gameInstanceExist)
		{
			//UE_LOG(LogTemp, Error, TEXT("UIManagerSubsystem: Dedicated server == %s"), *Outer->GetName());

		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("UIManagerSubsystem: client side == %s"), *Outer->GetName());

		}
	}
	else
	{
		gameInstanceExist = false;

	}
	

	return 	gameInstanceExist;

}

void ULomGameUIManagerSubsystem::RegisterAnchorPoint(ULomUIAnchorPoint* AnchorPoint)
{
	if (AnchorPoint)
	{
		anchorPoints.Add(AnchorPoint->PointTag, AnchorPoint);
	}
	//UE_LOG(LogTemp, Error, TEXT("AnchorPoint count == %i"),anchorPoints.Num());

}

void ULomGameUIManagerSubsystem::UnregisterAnchorPoint(ULomUIAnchorPoint* AnchorPoint)
{
	if (AnchorPoint)
	{
		anchorPoints.Remove(AnchorPoint->PointTag);
	}
}

void ULomGameUIManagerSubsystem::LayoutAdded()
{
	if (waitingWidgetsForLayout.Num() > 0)
	{
		TArray<FGameplayTag> keysToRemove;
		waitingWidgetsForLayout.GetKeys(keysToRemove);
		for (FGameplayTag& key : keysToRemove)
		{
			if (anchorPoints.Contains(key))
			{
				PushContentToAnchor(key, *waitingWidgetsForLayout.Find(key));
				waitingWidgetsForLayout.Remove(key);
			}
		}

	}
	
}

void ULomGameUIManagerSubsystem::RemoveWidget(UUserWidget* Widget)
{
	//UE_LOG(LogGameUI, Error, TEXT("remove widget == %s "), *Widget->GetName());
	if (addedWidgets.Contains(Widget))
	{
		ULomUIAnchorPoint* anchor = *addedWidgets.Find(Widget);
		anchor->RemoveChild(Widget);
	}
}




