// Fill out your copyright notice in the Description page of Project Settings.


#include "LomGameUIManagerSubsystem.h"
#include "LomGameUISettings.h"

DECLARE_LOG_CATEGORY_CLASS(LogGameUI, All, All);

void ULomGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	/*if (GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		return;
	}*/
	//UE_LOG(LogTemp, Error, TEXT("UIManagerSubsystem: initialization"));

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

void ULomGameUIManagerSubsystem::Deinitialize()
{
	TArray<FGuid> Handles;
	widgetRequests.GetKeys(Handles);
	for (const FGuid& Handle : Handles)
	{
		RemoveWidgetRequest(Handle);
	}
	anchorPoints.Empty();
	currentGameUIPolicy = nullptr;
	Super::Deinitialize();
}

void ULomGameUIManagerSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void ULomGameUIManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	//It must be done because servertravel crash the game because it can't find player controller or something else
	//FTimerHandle timerHandle;
	//GetWorld()->GetTimerManager().SetTimer(timerHandle,this, &ULomGameUIManagerSubsystem::AddLayout, 1.f, false);

	
	if (IsValid(currentGameUIPolicy))
	{
		currentGameUIPolicy->SetLayout();
	}

	

}

void ULomGameUIManagerSubsystem::PushContentToLayer(FGameplayTag layer, TSubclassOf<ULomCommonActivatableWidget> widgetClass, bool reset, bool lockedForPop)
{
	//if (!currentGameUIPolicy->GetCurrentLayout())
	//{
	//	currentGameUIPolicy->SetLayout();

	//}
	if (IsValid(currentGameUIPolicy) && currentGameUIPolicy->GetCurrentLayout())
	{
		currentGameUIPolicy->PushContentToLayer(layer, widgetClass, reset, lockedForPop);
		LayoutAdded();

	}

}

UUserWidget* ULomGameUIManagerSubsystem::PushContentToAnchor(FGameplayTag AnchorTag, TSubclassOf<UUserWidget> WidgetClass)
{
	return GetWidgetForRequest(RequestContentToAnchor(AnchorTag, WidgetClass));
}

FGuid ULomGameUIManagerSubsystem::RequestContentToAnchor(FGameplayTag AnchorTag, TSubclassOf<UUserWidget> WidgetClass, UObject* Owner)
{
	if (!AnchorTag.IsValid() || !WidgetClass || (Owner && !IsValid(Owner)))
	{
		return FGuid();
	}

	const FGuid Handle = FGuid::NewGuid();
	FLomAnchorWidgetRequest& Request = widgetRequests.Add(Handle);
	Request.AnchorTag = AnchorTag;
	Request.WidgetClass = WidgetClass;
	Request.Owner = Owner;
	Request.bHasOwner = Owner != nullptr;
	TryCreateWidgetRequest(Handle);
	return Handle;
}

UUserWidget* ULomGameUIManagerSubsystem::GetWidgetForRequest(FGuid RequestHandle) const
{
	const FLomAnchorWidgetRequest* Request = widgetRequests.Find(RequestHandle);
	return Request ? Request->Widget.Get() : nullptr;
}

void ULomGameUIManagerSubsystem::TryCreateWidgetRequest(FGuid RequestHandle)
{
	FLomAnchorWidgetRequest* Request = widgetRequests.Find(RequestHandle);
	if (!Request || Request->bCreating || Request->Widget)
	{
		return;
	}
	if (Request->bHasOwner && !Request->Owner.IsValid())
	{
		RemoveWidgetRequest(RequestHandle);
		return;
	}
	const TWeakObjectPtr<ULomUIAnchorPoint>* AnchorPtr = anchorPoints.Find(Request->AnchorTag);
	ULomUIAnchorPoint* Anchor = AnchorPtr ? AnchorPtr->Get() : nullptr;
	if (!IsValid(Anchor))
	{
		return;
	}

	Request->bCreating = true;
	Request->Anchor = Anchor;
	const TSubclassOf<UUserWidget> WidgetClass = Request->WidgetClass;
	UUserWidget* Widget = Anchor->AddChild(WidgetClass);

	// Construct callbacks can cancel requests or add others, invalidating map pointers.
	Request = widgetRequests.Find(RequestHandle);
	if (!Request)
	{
		if (IsValid(Anchor) && IsValid(Widget))
		{
			Anchor->RemoveChild(Widget);
		}
		return;
	}
	Request->bCreating = false;
	Request->Widget = Widget;
	if (!IsValid(Widget) || !IsValid(Anchor) || (Request->bHasOwner && !Request->Owner.IsValid()))
	{
		RemoveWidgetRequest(RequestHandle);
	}
}

void ULomGameUIManagerSubsystem::RemoveWidgetRequest(FGuid RequestHandle)
{
	FLomAnchorWidgetRequest Request;
	if (widgetRequests.RemoveAndCopyValue(RequestHandle, Request))
	{
		if (ULomUIAnchorPoint* Anchor = Request.Anchor.Get())
		{
			if (IsValid(Request.Widget))
			{
				Anchor->RemoveChild(Request.Widget);
			}
		}
	}
}

void ULomGameUIManagerSubsystem::RemoveWidgetsForOwner(UObject* Owner)
{
	if (!Owner)
	{
		return;
	}
	TArray<FGuid> Handles;
	for (const auto& Pair : widgetRequests)
	{
		if (Pair.Value.Owner == Owner)
		{
			Handles.Add(Pair.Key);
		}
	}
	for (const FGuid& Handle : Handles)
	{
		RemoveWidgetRequest(Handle);
	}
}

void ULomGameUIManagerSubsystem::PopFromLayer(FGameplayTag Layer, bool forcePop)
{
	if (IsValid(currentGameUIPolicy) && currentGameUIPolicy->GetCurrentLayout())
	{
		currentGameUIPolicy->PopFromLayer(Layer,forcePop);
	}
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
	if (IsValid(AnchorPoint) && AnchorPoint->PointTag.IsValid())
	{
		anchorPoints.Add(AnchorPoint->PointTag, AnchorPoint);
		LayoutAdded();
	}
	//UE_LOG(LogTemp, Error, TEXT("AnchorPoint count == %i"),anchorPoints.Num());

}

void ULomGameUIManagerSubsystem::UnregisterAnchorPoint(ULomUIAnchorPoint* AnchorPoint)
{
	if (AnchorPoint)
	{
		const TWeakObjectPtr<ULomUIAnchorPoint>* RegisteredAnchor = anchorPoints.Find(AnchorPoint->PointTag);
		if (RegisteredAnchor && RegisteredAnchor->Get() == AnchorPoint)
		{
			anchorPoints.Remove(AnchorPoint->PointTag);
		}
		TArray<FGuid> Handles;
		for (const auto& Pair : widgetRequests)
		{
			if (Pair.Value.Anchor == AnchorPoint)
			{
				Handles.Add(Pair.Key);
			}
		}
		for (const FGuid& Handle : Handles)
		{
			RemoveWidgetRequest(Handle);
		}
	}
}

void ULomGameUIManagerSubsystem::LayoutAdded()
{
	TArray<FGuid> Handles;
	widgetRequests.GetKeys(Handles);
	for (const FGuid& Handle : Handles)
	{
		TryCreateWidgetRequest(Handle);
	}
	
}

void ULomGameUIManagerSubsystem::AddLayout()
{
	if (IsValid(currentGameUIPolicy))
	{
		currentGameUIPolicy->SetLayout();
	}

}

void ULomGameUIManagerSubsystem::RemoveWidget(UUserWidget* Widget)
{
	//UE_LOG(LogGameUI, Error, TEXT("remove widget == %s "), *Widget->GetName());
	if (!IsValid(Widget))
	{
		return;
	}
	TArray<FGuid> Handles;
	for (const auto& Pair : widgetRequests)
	{
		const ULomUIAnchorPoint* Anchor = Pair.Value.Anchor.Get();
		// A widget can request removal in Construct, before AddChild returns it.
		const bool bRemovingDuringCreation = Pair.Value.bCreating && Anchor
			&& Pair.Value.WidgetClass == Widget->GetClass()
			&& Anchor->GetAllEntries().Contains(Widget);
		if (Pair.Value.Widget == Widget || bRemovingDuringCreation)
		{
			Handles.Add(Pair.Key);
		}
	}
	for (const FGuid& Handle : Handles)
	{
		RemoveWidgetRequest(Handle);
	}
}




