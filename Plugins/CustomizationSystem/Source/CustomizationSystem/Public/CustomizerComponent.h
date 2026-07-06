// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomizerComponent.generated.h"

class UCustomizerItemDefinition;
class UCustomizerAction;

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUSTOMIZATIONSYSTEM_API UCustomizerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomizerComponent();

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TArray<UCustomizerItemDefinition*> AllowedCategories;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UCustomizerItemDefinition*> InitialCustomizerItemDefinition;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<UCustomizerItemDefinition*> ItemDefinitionList;

	TMap<FName, TArray<UActorComponent*>> CacheComponent;

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
	void AddItemDefinition(UCustomizerItemDefinition* definition);
		
	UFUNCTION(BlueprintCallable)
	void AddAction(UCustomizerAction* action);

	UFUNCTION(BlueprintCallable)
	void ClearCache(FName key);

	UFUNCTION(BlueprintCallable)
	void AddCache(FName key,UActorComponent* cachedComponent);

	UFUNCTION(BlueprintCallable)
	TArray<UActorComponent*> GetCache(FName key);
};
