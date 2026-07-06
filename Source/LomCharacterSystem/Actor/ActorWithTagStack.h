// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/GameplayTagStack.h"

#include "ActorWithTagStack.generated.h"

UCLASS()
class LOMCHARACTERSYSTEM_API AActorWithTagStack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorWithTagStack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// TagStack
	UFUNCTION(BlueprintCallable)
	int32 GetTagStackCount(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	void AddTagStack(FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable)
	void OverrideTagStack(FGameplayTag Tag, int32 Count);

	UFUNCTION(BlueprintCallable)
	bool ContainsTagStack(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	void RemoveTagStack(FGameplayTag Tag, int32 Count);

	/*UFUNCTION(BlueprintCallable)
	TArray<FGameplayTagStack> GetAllTagStack();*/

	//End TagStack

private:

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TagStackContainer;
};
