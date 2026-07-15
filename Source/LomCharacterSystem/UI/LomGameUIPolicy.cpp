// Fill out your copyright notice in the Description page of Project Settings.


#include "LomGameUIPolicy.h"
#include "LomGameUIPrimaryLayout.h"
#include "LomGameUIManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"


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
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0); //GetWorld()->GetFirstPlayerController();
	UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (pc)
	{
		UE_LOG(LogTemp, Error, TEXT("plaer controller name = %s"), *pc->GetName());
		currentLayout = CreateWidget<ULomGameUIPrimaryLayout>(pc, DefaultLayoutClass);
		if (currentLayout)
		{
			UE_LOG(LogTemp, Error, TEXT("currentLayout spawned normally"));

			currentLayout->AddToPlayerScreen(100);

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("currentLayout not valid null"));

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("plaer controller nullptr"));

	}
	
	
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

TObjectPtr<ULomGameUIPrimaryLayout> ULomGameUIPolicy::GetCurrentLayout()
{
	return currentLayout;
}
