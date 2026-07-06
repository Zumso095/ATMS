// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Attribute/HealthAttributeSet.h"
#include "NativeGameplayTags.h"

#include "LomCharacter.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Block_Movement);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Block_Look);

class UObject;

UCLASS(Blueprintable)
class LOMCHARACTERSYSTEM_API ALomCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALomCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	// Called when the game starts or when spawned
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UAbilitySystemComponent* abilitySystemComponent;

	virtual void PossessedBy(AController* NewController) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	virtual void AddControllerPitchInput(float Val) override;
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerRollInput(float Val) override;



	
private:
	UPROPERTY()
	UHealthAttributeSet* HealthAttributeSet;
};
