// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LomHealthComponent.h"
#include "AbilitySystem/Attribute/HealthAttributeSet.h"
#include "AbilitySystem/Attribute/StaminaAttributeSet.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"


UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Elimination_Message, "Player.Elimination");

// Sets default values for this component's properties
ULomHealthComponent::ULomHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	// ...
	IsAlive = true;
}


// Called when the game starts
void ULomHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (GetOwner()->HasAuthority())
	{

		//UE_LOG(LogTemp, Error, TEXT("Trying to bind to health set"));
		currentASC = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
		if (currentASC)
		{
			//UE_LOG(LogTemp, Error, TEXT("ability system exists"));
			if (const UHealthAttributeSet* healthSet = currentASC->GetSet<UHealthAttributeSet>())
			{
				healthSet->OnDead.AddUObject(this, &ULomHealthComponent::OnDead);
				healthSet->OnReset.AddUObject(this, &ULomHealthComponent::OnResetHealth);
				healthSet->OnHealthChanged.AddUObject(this, &ULomHealthComponent::OnHealthChanged);

				//UE_LOG(LogTemp, Error, TEXT("health set exists"));

			}
		}
	}

	// Server-side bindings above stay authority-only. The cosmetic events below have to run on the
	// machine that hears the sound, so they are bound separately and driven by attribute replication.
	BindLocalAttributeEvents();
}

void ULomHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindLocalAttributeEvents();

	Super::EndPlay(EndPlayReason);
}

void ULomHealthComponent::BindLocalAttributeEvents()
{
	// A dedicated server has no local pawn and plays nothing.
	if (GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	LocalASC = Owner->FindComponentByClass<UAbilitySystemComponent>();
	if (!LocalASC)
	{
		return;
	}

	// GetGameplayAttributeValueChangeDelegate only touches a map keyed by the attribute, so it is
	// safe before the attribute sets have replicated in.
	HealthChangedHandle = LocalASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ULomHealthComponent::HandleHealthValueChanged);

	StaminaChangedHandle = LocalASC->GetGameplayAttributeValueChangeDelegate(UStaminaAttributeSet::GetStaminaAttribute())
		.AddUObject(this, &ULomHealthComponent::HandleStaminaValueChanged);

	ManaChangedHandle = LocalASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetManaAttribute())
		.AddUObject(this, &ULomHealthComponent::HandleManaValueChanged);

	// Seed the latches from the current values. On a client that has not received the attribute sets
	// yet every value reads as 0, so the attributes start out latched as depleted and the first
	// replication of the real values only clears the latch instead of firing a bogus event.
	bHealthDepleted = LocalASC->GetNumericAttribute(UHealthAttributeSet::GetHealthAttribute()) <= HealthDepletedThreshold;
	bStaminaDepleted = LocalASC->GetNumericAttribute(UStaminaAttributeSet::GetStaminaAttribute()) <= StaminaDepletedThreshold;
	bManaDepleted = LocalASC->GetNumericAttribute(UHealthAttributeSet::GetManaAttribute()) <= ManaDepletedThreshold;
}

void ULomHealthComponent::UnbindLocalAttributeEvents()
{
	if (LocalASC)
	{
		LocalASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute()).Remove(HealthChangedHandle);
		LocalASC->GetGameplayAttributeValueChangeDelegate(UStaminaAttributeSet::GetStaminaAttribute()).Remove(StaminaChangedHandle);
		LocalASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetManaAttribute()).Remove(ManaChangedHandle);
	}

	HealthChangedHandle.Reset();
	StaminaChangedHandle.Reset();
	ManaChangedHandle.Reset();
	LocalASC = nullptr;
}

bool ULomHealthComponent::IsLocalCosmeticContext() const
{
	// Checked at broadcast time rather than at bind time: on a client the pawn replicates before its
	// controller, so IsLocallyControlled is still false while BeginPlay runs.
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());

	return OwnerPawn != nullptr && OwnerPawn->IsLocallyControlled();
}

void ULomHealthComponent::HandleHealthValueChanged(const FOnAttributeChangeData& Data)
{
	const float Delta = Data.OldValue - Data.NewValue;
	if (Delta >= DamageTakenMinDelta && IsLocalCosmeticContext())
	{
		OnDamageTakenLocal.Broadcast(Delta, Data.NewValue);
	}

	UpdateDepletedLatch(Data.NewValue, HealthDepletedThreshold, bHealthDepleted, OnHealthDepletedLocal);
}

void ULomHealthComponent::HandleStaminaValueChanged(const FOnAttributeChangeData& Data)
{
	UpdateDepletedLatch(Data.NewValue, StaminaDepletedThreshold, bStaminaDepleted, OnStaminaDepletedLocal);
}

void ULomHealthComponent::HandleManaValueChanged(const FOnAttributeChangeData& Data)
{
	UpdateDepletedLatch(Data.NewValue, ManaDepletedThreshold, bManaDepleted, OnManaDepletedLocal);
}

void ULomHealthComponent::UpdateDepletedLatch(float NewValue, float Threshold, bool& bLatch, FOnLocalAttributeDepletedDelegate& Event)
{
	const bool bDepleted = NewValue <= Threshold;
	if (bDepleted == bLatch)
	{
		// Keeps periodic costs such as GE_J_Sprint_Cost (0.1 s) from spamming the event.
		return;
	}

	bLatch = bDepleted;

	if (bDepleted && IsLocalCosmeticContext())
	{
		Event.Broadcast();
	}
}


// Called every frame
void ULomHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULomHealthComponent::OnDead(AActor* Instigator, const FGameplayEffectSpec*, float Magnitude, float OldValude, float NewValue)
{
	FAttributeChangedMessage message;
	message.Instigator = Instigator;
	message.Target = currentASC->GetAvatarActor();
	message.Magnitude = Magnitude;
	//UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(TAG_Elimination_Message, message);
	IsAlive = false;
	OnDeadMulticast(message);
}

void ULomHealthComponent::OnHealthChanged(AActor* Instigator, const FGameplayEffectSpec* spec, float Magnitude, float OldValude, float NewValue)
{
	OnHealthChangedD.Broadcast(Instigator, spec->GetEffectContext().GetEffectCauser(), Magnitude);
}

void ULomHealthComponent::OnResetHealth()
{
	IsAlive = true;
}

void ULomHealthComponent::OnDeadMulticast_Implementation(FAttributeChangedMessage Message)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("onDeadMulticast works"));
	UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(TAG_Elimination_Message, Message);
	OnCharacterDead.Broadcast();
}



void ULomHealthComponent::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, IsAlive);
}



