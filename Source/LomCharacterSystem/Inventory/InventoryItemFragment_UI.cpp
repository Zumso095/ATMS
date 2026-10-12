// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemFragment_UI.h"
#include "UI/LomGameUIManagerSubsystem.h"

void UInventoryItemFragment_UI::AddWidgets()
{
	ULomGameUIManagerSubsystem* uiManager = ULomGameUIManagerSubsystem::Get();
	if (!uiManager)
	{
		return;
	}
	for (FUIFragmentItemContainer& widget : WidgetsToAdd)
	{
		if (widget.AnchorPoint.IsValid() && widget.WidgetClass)
		{
			uiManager->RequestContentToAnchor(widget.AnchorPoint, widget.WidgetClass, this);
		}
	}
}

void UInventoryItemFragment_UI::RemoveWidgets()
{
	ULomGameUIManagerSubsystem* uiManager = ULomGameUIManagerSubsystem::Get();
	if (uiManager)
	{
		uiManager->RemoveWidgetsForOwner(this);
	}
}


