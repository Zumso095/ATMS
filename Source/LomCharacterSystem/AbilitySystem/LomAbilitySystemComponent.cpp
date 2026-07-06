// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "LomGlobalAbilitySubsystem.h"
///#include "LomAbilitySystemComponent.h"

void ULomAbilitySystemComponent::PressInputWithTag(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Display, TEXT("Ability activated with tag = %s"), *InputTag.ToString());

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& item : ActivatableAbilities.Items)
	{
		if (item.Ability && item.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			TryActivateAbility(item.Handle);

			item.InputPressed = true;
			AbilitySpecInputPressed(item);
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "dynamic tag works");
			//UE_LOG(LogTemp, Display, TEXT("Ability activated with tag = %s"), *InputTag.ToString());
			
			
		}
	}
}

void ULomAbilitySystemComponent::ReleaseInputWithTag(FGameplayTag InputTag)
{
	//ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& item : ActivatableAbilities.Items)
	{
		if (item.Ability && item.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			item.InputPressed = false;

			AbilitySpecInputReleased(item);
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, "dynamic tag works");
			//TryActivateAbility(item.Handle);
			//UE_LOG(LogTemp, Display, TEXT("Ability released with tag = %s"), *InputTag.ToString());
		}
	}
}

void ULomAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	
	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

			// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, OriginalPredictionKey);
	}
}

void ULomAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	// We don't support UGameplayAbility::bReplicateInputDirectly.
	// Use replicated events instead so that the WaitInputRelease ability task works.
	if (Spec.IsActive())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
			const UGameplayAbility* Instance = Spec.GetPrimaryInstance();
		FPredictionKey OriginalPredictionKey = Instance ? Instance->GetCurrentActivationInfo().GetActivationPredictionKey() : Spec.ActivationInfo.GetActivationPredictionKey();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS

			// Invoke the InputReleased event. This is not replicated here. If someone is listening, they may replicate the InputReleased event to the server.
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, OriginalPredictionKey);
		/*UE_LOG(LogTemp, Warning, TEXT("ScopedKey=%s, ActivationKey=%s"),
			*ScopedPredictionKey.ToString(),
			*OriginalPredictionKey.ToString());*/
	}

		
	
}

void ULomAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	ULomGlobalAbilitySubsystem* globalAbilitySubsystem = GetWorld()->GetSubsystem<ULomGlobalAbilitySubsystem>();
	if (globalAbilitySubsystem)
	{
		globalAbilitySubsystem->AddAbilitySystemComponent(this);
	}
}

void ULomAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ULomGlobalAbilitySubsystem* globalAbilitySubsystem = GetWorld()->GetSubsystem<ULomGlobalAbilitySubsystem>();
	if (globalAbilitySubsystem)
	{
		globalAbilitySubsystem->RemoveAbilitySystemComponent(this);
	}
	Super::EndPlay(EndPlayReason);
}
