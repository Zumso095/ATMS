// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"

#include "LomHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedDelegate, AActor*, Instigator, AActor*, Target, float, Magnitude);

// Local cosmetic events (sounds, UI). Broadcast only on the machine that locally controls
// the owning pawn: the owning client, or the host on a listen server.
// They are driven by attribute replication, so no Instigator / EffectSpec is available here.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLocalDamageTakenDelegate, float, Amount, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLocalAttributeDepletedDelegate);

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

	// --- Local (owner-only) attribute events, meant for cosmetics such as sound ---

	UPROPERTY(BlueprintAssignable, Category = "Lom|Local Events")
	FOnLocalDamageTakenDelegate OnDamageTakenLocal;

	UPROPERTY(BlueprintAssignable, Category = "Lom|Local Events")
	FOnLocalAttributeDepletedDelegate OnHealthDepletedLocal;

	UPROPERTY(BlueprintAssignable, Category = "Lom|Local Events")
	FOnLocalAttributeDepletedDelegate OnStaminaDepletedLocal;

	UPROPERTY(BlueprintAssignable, Category = "Lom|Local Events")
	FOnLocalAttributeDepletedDelegate OnManaDepletedLocal;

	// An attribute counts as depleted once it is at or below this value. The matching event fires
	// once on the downward crossing and only repeats after the attribute rises above the threshold.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lom|Local Events")
	float HealthDepletedThreshold = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lom|Local Events")
	float StaminaDepletedThreshold = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lom|Local Events")
	float ManaDepletedThreshold = 0.0f;

	// Health drops smaller than this are ignored as damage (replication noise, rounding).
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Lom|Local Events")
	float DamageTakenMinDelta = 0.01f;

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void BindLocalAttributeEvents();
	void UnbindLocalAttributeEvents();

	// True only where the owning pawn is locally controlled.
	bool IsLocalCosmeticContext() const;

	void HandleHealthValueChanged(const FOnAttributeChangeData& Data);
	void HandleStaminaValueChanged(const FOnAttributeChangeData& Data);
	void HandleManaValueChanged(const FOnAttributeChangeData& Data);

	void UpdateDepletedLatch(float NewValue, float Threshold, bool& bLatch, FOnLocalAttributeDepletedDelegate& Event);

private:

	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> LocalASC = nullptr;

	FDelegateHandle HealthChangedHandle;
	FDelegateHandle StaminaChangedHandle;
	FDelegateHandle ManaChangedHandle;

	bool bHealthDepleted = false;
	bool bStaminaDepleted = false;
	bool bManaDepleted = false;
};