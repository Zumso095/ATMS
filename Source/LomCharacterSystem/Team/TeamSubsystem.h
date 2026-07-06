// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TeamSubsystem.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ELomTeamComparison: uint8
{
	TheSame,
	Different,
	Wrong
};

UCLASS()
class LOMCHARACTERSYSTEM_API UTeamSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()


public:
	

	UFUNCTION(BlueprintCallable)
	void SetTeam(AActor* actor, int team);
	
	UFUNCTION(BlueprintCallable, meta = (ExpandEnumAsExecs = ReturnValue))
	ELomTeamComparison CompareTeam(AActor* actorA, AActor* actorB);

private:
	TMap<AActor*, int32> Teams;
};
