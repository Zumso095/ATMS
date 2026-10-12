// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/LomGameplayAbility_WithWidget.h"
#include "LomGameplayAbility_WithWidget.h"
#include "UI/LomGameUIManagerSubsystem.h"

void ULomGameplayAbility_WithWidget::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	bWidgetsRemoved = false;
	Super::OnGiveAbility(ActorInfo, Spec);

	if (!bWidgetsRemoved && GetWorld() && IsLocallyControlled() && !GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		ULomGameUIManagerSubsystem* uiManager = GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>();
		if (!uiManager)
		{
			return;
		}

		for (TPair<FGameplayTag,TSubclassOf<UUserWidget>> widget : Widgets)
		{
			if (bWidgetsRemoved)
			{
				break;
			}
			if (widget.Key.IsValid() && widget.Value)
			{
				uiManager->RequestContentToAnchor(widget.Key, widget.Value, this);
			}
		}
	}
}

void ULomGameplayAbility_WithWidget::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	bWidgetsRemoved = true;
	Super::OnRemoveAbility(ActorInfo, Spec);
}
