// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LomInventoryItemDefinition.h"
#include "System/GameplayTagStack.h"
#include "LomInventoryItemInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class LOMCHARACTERSYSTEM_API ULomInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere,Replicated,BlueprintReadWrite)
	int32 Amount;
	
	void SetItemDefinition(TSubclassOf<ULomInventoryItemDefinition> ItemDefinition);

	UFUNCTION(BlueprintCallable)
	TSubclassOf<ULomInventoryItemDefinition> GetItemDefinition();

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
	//End TagStack

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

private:
	UPROPERTY(Replicated)
	TSubclassOf<ULomInventoryItemDefinition> itemDefinition;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TagStackContainer;
};
