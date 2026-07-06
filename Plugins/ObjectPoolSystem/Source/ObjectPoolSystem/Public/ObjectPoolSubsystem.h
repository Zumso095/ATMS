// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

/**
*
 */
USTRUCT(BlueprintType)
struct FPoolActorArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Actors;
};

UCLASS()
class OBJECTPOOLSYSTEM_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FPoolActorArray> PoolList;
	//TArray<AActor*> testimArray;

public:
	//UFUNCTION(BlueprintCallable)
	//void Pop();
	//static UObjectPoolSubsystem* Get(UObject* WorldContext) {
	//	/*if (WorldContext->GetWorld())
	//	{
	//		if (WorldContext->instance GetWorld()->GetSubsystem<UObjectPoolSubsystem>()) {
	//			return WorldContext->GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

	//		}

	//	}*/

	//	return GEngine->GetWorld()
	//}

	/*UFUNCTION(BlueprintCallable)
	static AActor* SpawnActorObjectPool(TSubclassOf<AActor> actorClass, FTransform actorTransform, AActor* owner = nullptr, APawn* instigator = nullptr);
	*/
	UFUNCTION(BlueprintCallable)
	AActor* SpawnOrPop(TSubclassOf<AActor> actorClass, FTransform actorTransform, AActor* owner = nullptr, APawn* instigator = nullptr);

	UFUNCTION(BlueprintCallable)
	void ReserveActors(TSubclassOf<AActor> actorClass, int32 count = 1);

	UFUNCTION(BlueprintCallable)
	void ShowObjectPoolDebug();

	
};
