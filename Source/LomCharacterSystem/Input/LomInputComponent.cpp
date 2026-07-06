// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/LomInputComponent.h"
#include "Input/LomInputConfig.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "Player/LomPlayerController.h"

void ULomInputComponent::BindInputConfig(ULomInputConfig* Config)
{
	if (Config) 
	{
		//UE_LOG(LogTemp, Display, TEXT("config file correct"));
		for (FLomInputAction& action : Config->InputActions)
		{
			BindAction(action.InputAction, ETriggerEvent::Started, this, &ULomInputComponent::PressInputTag, action.InputTag);
			BindAction(action.InputAction, ETriggerEvent::Completed, this, &ULomInputComponent::ReleaseInputTag, action.InputTag);


		}
	}
}

void ULomInputComponent::PressInputTag(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Display, TEXT("pressed  %s"),*InputTag.ToString());
	if (ALomPlayerController* pc = Cast<ALomPlayerController>(GetOwner()))
	{
		if (pc->GetPawn())
		{
			ULomAbilitySystemComponent* abilitySystemComp = pc->GetPawn()->GetComponentByClass<ULomAbilitySystemComponent>();
			if (abilitySystemComp)
			{
				abilitySystemComp->PressInputWithTag(InputTag);
				
			}
		}
	}
	
}

void ULomInputComponent::ReleaseInputTag(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Display, TEXT("released %s"), *InputTag.ToString());
	if (ALomPlayerController* pc = Cast<ALomPlayerController>(GetOwner()))
	{
		if (pc->GetPawn())
		{
			ULomAbilitySystemComponent* abilitySystemComp = pc->GetPawn()->GetComponentByClass<ULomAbilitySystemComponent>();
			if (abilitySystemComp)
			{
				abilitySystemComp->ReleaseInputWithTag(InputTag);

			}
		}
	}

}
