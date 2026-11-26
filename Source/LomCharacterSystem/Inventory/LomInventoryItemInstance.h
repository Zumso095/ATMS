// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LomInventoryItemDefinition.h"
#include "System/GameplayTagStack.h"
#include "LomInventoryItemInstance.generated.h"

class ULomInventoryItemInstance;

// Fires when a tag stack count on this item changes (e.g. ammo).
// Server: right after Add/Override/RemoveTagStack. Clients: when the replicated value arrives.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnItemTagStackChanged, ULomInventoryItemInstance*, ItemInstance, FGameplayTag, Tag, int32, NewCount, int32, OldCount);

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

	UPROPERTY(BlueprintAssignable)
	FOnItemTagStackChanged OnTagStackChanged;
	//End TagStack

	virtual void GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

private:
	UPROPERTY(Replicated)
	TSubclassOf<ULomInventoryItemDefinition> itemDefinition;

	UPROPERTY(ReplicatedUsing = OnRep_TagStackContainer)
	FGameplayTagStackContainer TagStackContainer;

	UFUNCTION()
	void OnRep_TagStackContainer(const FGameplayTagStackContainer& OldContainer);

	void BroadcastIfChanged(FGameplayTag Tag, int32 OldCount);
};
