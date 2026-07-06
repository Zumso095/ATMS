// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/LomUIAnchorPoint.h"
#include "UI/LomGameUIManagerSubsystem.h"

#define LOCTEXT_NAMESPACE "LomUIAnchor"

UUserWidget* ULomUIAnchorPoint::AddChild(TSubclassOf<UUserWidget> WidgetClass)
{
	if (WidgetClass)
	{
		return CreateEntryInternal(WidgetClass);

	}
	return nullptr;
}

void ULomUIAnchorPoint::RemoveChild(UUserWidget* Widget)
{
	if (Widget)
	{
		RemoveEntryInternal(Widget,true);
	}
}

void ULomUIAnchorPoint::RegisterAnchor()
{
	
	if (GetWorld())
	{
		if (ULomGameUIManagerSubsystem* uiManager = GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>())
		{
			uiManager->RegisterAnchorPoint(this);
			
		}
	}
	
}

void ULomUIAnchorPoint::ResetAnchor()
{
	if (GetWorld())
	{
		if (ULomGameUIManagerSubsystem* uiManager = GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>())
		{
			uiManager->UnregisterAnchorPoint(this);
		}
	}
	
}

TSharedRef<SWidget> ULomUIAnchorPoint::RebuildWidget()
{
	//UE_LOG(LogTemp, Error, TEXT("rebuildWidget"));

	if (!IsDesignTime() && PointTag.IsValid())
	{
		RegisterAnchor();
	}

	auto messageTextLambda = [this]()
		{
			return FText::Format(LOCTEXT("LomAnchorPointKey", "Anchor\n {0}"), FText::FromName(PointTag.GetTagName()));
		};

	if (IsDesignTime())
	{
		TSharedRef<SOverlay> Message = SNew(SOverlay);
		Message->AddSlot().Padding(5).HAlign(HAlign_Center).VAlign(VAlign_Center)
			[SNew(STextBlock)
			.Justification(ETextJustify::Center)
			.Text_Lambda(messageTextLambda)];
		return Message;
	}

	return Super::RebuildWidget();
}
#if WITH_EDITOR	
void ULomUIAnchorPoint::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
}
#endif
void ULomUIAnchorPoint::ReleaseSlateResources(bool bReleaseChildren)
{
	//UE_LOG(LogTemp, Error, TEXT("Release slate resources "));
	ResetInternal();
	ResetAnchor();
	Super::ReleaseSlateResources(bReleaseChildren);
}

#undef LOCTEXT_NAMESPACE