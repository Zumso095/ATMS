// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemFragment_UI.h"
#include "UI/LomGameUIManagerSubsystem.h"

void UInventoryItemFragment_UI::AddWidgets()
{
	ULomGameUIManagerSubsystem* uiManager = ULomGameUIManagerSubsystem::Get();
	for (FUIFragmentItemContainer& widget : WidgetsToAdd)
	{
		if (widget.AnchorPoint.IsValid() && widget.WidgetClass)
		{
			addedWidgets.Add(uiManager->PushContentToAnchor(widget.AnchorPoint, widget.WidgetClass));
		}
	}
}

void UInventoryItemFragment_UI::RemoveWidgets()
{
	ULomGameUIManagerSubsystem* uiManager = ULomGameUIManagerSubsystem::Get();
	for (UUserWidget* widget : addedWidgets)
	{
		if (widget)
		{
			uiManager->RemoveWidget(widget);
		}
	}
}


