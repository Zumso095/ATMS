// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"

#include "LomHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedDelegate, AActor*, Instigator, AActor*, Target, float, Magnitude);

USTRUCT(BlueprintType)
struct FAttributeChangedMessage
{
	GENERATED_BODY()

	// 
	UPROPERTY(BlueprintReadWrite, Category = "Message")
	AActor* Instigator = nullptr;

	// 
	UPROPERTY(BlueprintReadWrite, Category = "Message")
	AActor* Target = nullptr;

	// 
	UPROPERTY(BlueprintReadWrite, Category = "Message")
	float Magnitude = 0.0f;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LOMCHARACTERSYSTEM_API ULomHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULomHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UAbilitySystemComponent* currentASC;

	void OnDead(AActor* Instigator, const FGameplayEffectSpec*, float Magnitude, float OldValude, float NewValue);
	void OnHealthChanged(AActor* Instigator, const FGameplayEffectSpec* spec, float Magnitude, float OldValude, float NewValue);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void OnDeadMulticast(FAttributeChangedMessage Message);

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool IsAlive;

	void OnResetHealth();

	UPROPERTY(BlueprintAssignable)
	FOnDeadDelegate OnCharacterDead;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChangedD;
};