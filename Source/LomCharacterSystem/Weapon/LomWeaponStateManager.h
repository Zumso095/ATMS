// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"


#include "LomWeaponStateManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetFoundDelegate, AActor*, Target, FVector, Direction);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOMCHARACTERSYSTEM_API ULomWeaponStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULomWeaponStateManager();

	UPROPERTY(EditAnywhere)
	double AngleDot;

	UFUNCTION(BlueprintCallable)
	AActor* GetTarget() { return currentTarget; }

	void FindTarget();
	FVector GetAimTargetLocation(AActor* target);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* currentTarget;

	UPROPERTY(BlueprintAssignable)
	FOnTargetFoundDelegate OnTargetFound;
};
