// Fill out your copyright notice in the Description page of Project Settings.


#include "LomGameUIPolicy.h"
#include "LomGameUIPrimaryLayout.h"
#include "LomGameUIManagerSubsystem.h"


UWorld* ULomGameUIPolicy::GetWorld() const
{
	return GetOuter()->GetWorld();
	
}

void ULomGameUIPolicy::SetLayout(ULocalPlayer* newLocalPlayer)
{
	currentLayout = CreateWidget<ULomGameUIPrimaryLayout>(newLocalPlayer->GetPlayerController(GetWorld()), DefaultLayoutClass);
	currentLayout->AddToPlayerScreen(100);
}

void ULomGameUIPolicy::SetLayout()
{
	currentLayout = CreateWidget<ULomGameUIPrimaryLayout>(GetWorld()->GetFirstPlayerController(), DefaultLayoutClass);
	currentLayout->AddToPlayerScreen(100);
	//GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeDataBase)
	//UE_LOG(LogTemp, Error, TEXT("%s"), TEXT("layout setted"));

}

void ULomGameUIPolicy::PushContentToLayer(FGameplayTag Layer, TSubclassOf<ULomCommonActivatableWidget> WidgetClass, bool reset, bool lockedForPop)
{
	
	currentLayout->PushContentToLayer(Layer, WidgetClass, reset,lockedForPop);
}

void ULomGameUIPolicy::PopFromLayer(FGameplayTag Layer, bool forcePop)
{
	currentLayout->PopFromLayer(Layer,forcePop);
}