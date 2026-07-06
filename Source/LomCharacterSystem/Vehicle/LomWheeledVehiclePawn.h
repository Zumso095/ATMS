// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "WheeledVehiclePawn.h"
#include "AbilitySystem/Attribute/HealthAttributeSet.h"
#include "LomWheeledVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ALomWheeledVehiclePawn : public AWheeledVehiclePawn, public IAbilitySystemInterface
{
	GENERATED_BODY()
	ALomWheeledVehiclePawn();

public:
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayreason) override;
	
public:
	virtual void PossessedBy(AController* NewController);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAbilitySystemComponent* abilitySystemComponent;

	UFUNCTION()
	void ReadyForInput();
private:
	UPROPERTY()
	UHealthAttributeSet* HealthAttributeSet;
};
