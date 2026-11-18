// Fill out your copyright notice in the Description page of Project Settings.

#include "LomGameInstance.h"

#include "UI/LomGameUIManagerSubsystem.h"



int32 ULomGameInstance::AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId)
{
	int32 result = Super::AddLocalPlayer(NewPlayer, UserId);
	//const ULomGameUIManagerSubsystem* ms = GetSubsystem<ULomGameUIManagerSubsystem>();
	//GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>()->ShowMenu(NewPlayer);
	return result;
}

ULomGameInstance::ULomGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bAutoJoinSessionOnAcceptedUserInviteReceived = true;
	bAutoTravelOnAcceptedUserInviteReceived = true;
}
