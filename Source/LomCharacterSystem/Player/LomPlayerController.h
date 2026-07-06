// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ALomPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	static const FName NAME_ReadyForInput;

	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void AcknowledgePossession(class APawn* P) override;
	
	
	void GameModeLoadCompleted();
};
