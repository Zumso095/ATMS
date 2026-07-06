// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Pawn/LomPawnData.h"
#include "LomGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ALomGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALomGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameModeManager();

	UFUNCTION(BlueprintCallable)
	void SpawnActorFromPawnData(ULomPawnData* PawnData, FTransform SpawnTransform);

	UFUNCTION(BlueprintCallable)
	void SetGameModeDefinition();
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
	void IsGameModeReady();

};
