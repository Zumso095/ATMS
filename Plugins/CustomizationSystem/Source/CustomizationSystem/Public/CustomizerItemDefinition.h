// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CustomizerItemDefinition.generated.h"

/**
 * 
 */

UCLASS(Blueprintable,DefaultToInstanced, EditInlineNew)
class CUSTOMIZATIONSYSTEM_API UCustomizerAction : public UObject
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginPlay(AActor* owner,UCustomizerItemDefinition* itemDefinition,class UCustomizerComponent* customizerComponent);
};

UCLASS(Blueprintable,DefaultToInstanced, EditInlineNew)
class CUSTOMIZATIONSYSTEM_API UCustomizerFragment : public UObject
{
	GENERATED_BODY()
};

UCLASS()
class CUSTOMIZATIONSYSTEM_API UCustomizerItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Description)
	FName ProjectName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Description)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Description)
	FName PrimaryKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Description)
	FName SecondaryKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Definition,Instanced)
	TArray<TObjectPtr<UCustomizerFragment>> Fragments;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Definition,Instanced)
	TArray<TObjectPtr<UCustomizerAction>> Actions;

public:
	UFUNCTION(BlueprintCallable)
	UCustomizerFragment* FindFragmentByClass(TSubclassOf<UCustomizerFragment> fragmentClass);
};
