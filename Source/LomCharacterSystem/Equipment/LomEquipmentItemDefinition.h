// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LomEquipmentItemDefinition.generated.h"

/**
 * 
 */
class ULomEquipmentItemInstance;
class ULomAbilitySet;

USTRUCT(BlueprintType)
struct FLomEquipmentActorPresetToSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName SocketToAttach;

	UPROPERTY(EditDefaultsOnly)
	FTransform Offset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ActorClass;
};

UCLASS(Blueprintable)
class LOMCHARACTERSYSTEM_API ULomEquipmentItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	/*UPROPERTY(EditDefaultsOnly)
	FName Name;*/

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<ULomEquipmentItemInstance> EquipmentItemInstanceClass;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TObjectPtr<ULomAbilitySet> AbilitySet;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FLomEquipmentActorPresetToSpawn> ActorsToSpawn;
};
