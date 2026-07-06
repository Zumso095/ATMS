// Fill out your copyright notice in the Description page of Project Settings.


#include "LomGameUIPrimaryLayout.h"
//#include "CommonActivatableWidget.h"
//#include "Widget/LomCommonActivatableWidget.h"
#include "TimerManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"


//UWidget* ULomGameUIPrimaryLayout::NativeGetDesiredFocusTarget() const
//{
//	return nullptr;
//}

void ULomGameUIPrimaryLayout::RegisterLayer(FGameplayTag layerTag, UCommonActivatableWidgetContainerBase* WidgetContainer)
{
	if (layerTag.IsValid() && WidgetContainer)
	{
		Layers.Add(layerTag, WidgetContainer);
	}
	
}

void ULomGameUIPrimaryLayout::PushContentToLayer(FGameplayTag layer, TSubclassOf<ULomCommonActivatableWidget> WidgetClass, bool reset, bool LockedForPop)
{
	UCommonActivatableWidgetContainerBase* container = Layers.FindRef(layer);
	if (container)
	{
		if (!reset && container->GetActiveWidget())
		{
			/*UE_LOG(LogTemp, Error, TEXT("ActiveWidget exists"));
			UE_LOG(LogTemp, Error, TEXT("A %s"), *container->GetActiveWidget()->StaticClass()->GetName());
			UE_LOG(LogTemp, Error, TEXT("A %s"), *WidgetClass->GetName());*/
			//container->GetActiveWidget()->WidgetTree->GetAllWidgets()
			if (container->GetActiveWidget()->GetClass() == WidgetClass)
			{
				return;

			}
		}
		UCommonActivatableWidget* addedActiveWidget = container->AddWidget(WidgetClass);
		//container->curso
		//UE_LOG(LogTemp, Error, TEXT("added widget %s"), *addedActiveWidget->GetName());
		ULomCommonActivatableWidget* addedCustomWidget = Cast<ULomCommonActivatableWidget>(addedActiveWidget);
		if (addedCustomWidget)
		{
			if (LockedForPop)
			{
				//ULomCommonActivatableWidget* addedCustomWidget = Cast<ULomCommonActivatableWidget>(addedActiveWidget);
				//UE_LOG(LogTemp, Error, TEXT("cast %s"), *addedCustomWidget->GetName());			
					//UE_LOG(LogTemp, Error, TEXT("locked"));
					addedCustomWidget->LockedForPop = LockedForPop;
				
			}
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = addedCustomWidget->ShowCursor;
			

			FTimerHandle timerHandle;
			FTimerDelegate timerDelegate;
			timerDelegate.BindUObject(this, &ULomGameUIPrimaryLayout::SetInputMode, addedCustomWidget->InputMode, addedCustomWidget->HideCursorDuringCapture, addedCustomWidget->ShowCursor);
			//UE_LOG(LogTemp, Error, TEXT("timer %s"), *timerHandle.ToString());

			//SetInputMode(timerHandle, addedCustomWidget->InputMode);
			//timerDelegate.BindUFunction(this, "SetInputMode", timerHandle, addedCustomWidget->InputMode);

			GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, .1f, false);
			//GetWorld()->GetTimerManager().SetTimerForNextTick(timerDelegate);
			
			//GetWorld()->GetTimerManager().ClearTimer(timerHandle);
			//UE_LOG(LogTemp, Error, TEXT("timer %s"), *timerHandle.ToString());


		}
		

	}
}
void ULomGameUIPrimaryLayout::SetInputMode(EInputMode InputMode, bool HideCursorDuringCapture, bool ShowCursor)
{
	

	switch (InputMode)
	{

	case EInputMode::GameAndUI:
	{
		FInputModeGameAndUI inputGameAndUIConfig = FInputModeGameAndUI();
		inputGameAndUIConfig.SetHideCursorDuringCapture(HideCursorDuringCapture);
		GetWorld()->GetFirstPlayerController()->SetInputMode(inputGameAndUIConfig);
		break;
	}
		
	case EInputMode::UIOnly:
	{
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		break;
	}
		

	case EInputMode::GameOnly:
	{
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		break;
	}
		
	}
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = ShowCursor;
}
void ULomGameUIPrimaryLayout::PopFromLayer(FGameplayTag Layer, bool forcePop)
{
	UCommonActivatableWidgetContainerBase* container = Layers.FindRef(Layer);
	if (container)
	{
		if (container->GetActiveWidget())
		{
			if (ULomCommonActivatableWidget* activeCustomWidget = Cast<ULomCommonActivatableWidget>(container->GetActiveWidget()))
			{
				//UE_LOG(LogTemp, Error, TEXT("works"));

				if (!forcePop && activeCustomWidget->LockedForPop)
				{
					return;
				}
			}
			container->GetActiveWidget()->DeactivateWidget();
		}
	
	}
}


