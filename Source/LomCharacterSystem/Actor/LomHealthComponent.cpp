// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LomHealthComponent.h"
#include "AbilitySystem/Attribute/HealthAttributeSet.h"
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

	// The bindings above are authority-only. Clients get nothing from them, so they listen to
	// attribute replication instead and raise the same OnHealthChangedD event.
	BindClientHealthEvents();
}

void ULomHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindClientHealthEvents();

	Super::EndPlay(EndPlayReason);
}

void ULomHealthComponent::BindClientHealthEvents()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	// Authority already broadcasts OnHealthChangedD from the attribute set callback, with the full
	// Instigator / EffectSpec. Skipping authority here is what keeps a listen-server host from
	// raising the event twice for its own pawn.
	if (Owner->HasAuthority())
	{
		return;
	}

	ClientASC = Owner->FindComponentByClass<UAbilitySystemComponent>();
	if (!ClientASC)
	{
		return;
	}

	// GetGameplayAttributeValueChangeDelegate only touches a map keyed by the attribute, so it is
	// safe to bind before the attribute sets have replicated in.
	ClientHealthChangedHandle = ClientASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ULomHealthComponent::HandleHealthReplicated);
}

void ULomHealthComponent::UnbindClientHealthEvents()
{
	if (ClientASC)
	{
		ClientASC->GetGameplayAttributeValueChangeDelegate(UHealthAttributeSet::GetHealthAttribute()).Remove(ClientHealthChangedHandle);
	}

	ClientHealthChangedHandle.Reset();
	ClientASC = nullptr;
}

void ULomHealthComponent::HandleHealthReplicated(const FOnAttributeChangeData& Data)
{
	// Cosmetics only, and only for the local player. Checked here rather than at bind time because
	// a client replicates the pawn before its controller, so IsLocallyControlled still lies during
	// BeginPlay.
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn || !OwnerPawn->IsLocallyControlled())
	{
		return;
	}

	// Same sign convention as the authority path, which passes the GameplayEffect modifier
	// magnitude: negative for damage, positive for healing.
	const float Magnitude = Data.NewValue - Data.OldValue;
	if (FMath::IsNearlyZero(Magnitude))
	{
		return;
	}

	// Instigator and effect causer exist only in the GameplayEffect callback on the server.
	OnHealthChangedD.Broadcast(nullptr, GetOwner(), Magnitude);
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



