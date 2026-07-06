// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "LomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, FPlatformUserId UserId) override;
	
};
