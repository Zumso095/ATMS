// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LomHealthComponent.h"
#include "AbilitySystem/Attribute/HealthAttributeSet.h"
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



