// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/LomGameplayAbility_WithWidget.h"
#include "LomGameplayAbility_WithWidget.h"
#include "UI/LomGameUIManagerSubsystem.h"

void ULomGameplayAbility_WithWidget::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (IsLocallyControlled() && !GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		ULomGameUIManagerSubsystem* uiManager = GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>();

		for (TPair<FGameplayTag,TSubclassOf<UUserWidget>> widget : Widgets)
		{
			if (widget.Key.IsValid() && widget.Value)
			{
				addedWidgets.Add(uiManager->PushContentToAnchor(widget.Key, widget.Value));
			}
		}
	}
}

void ULomGameplayAbility_WithWidget::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (IsLocallyControlled() && !GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		ULomGameUIManagerSubsystem* uiManager = GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>();


		for (UUserWidget* widget : addedWidgets)
		{
			uiManager->RemoveWidget(widget);
		}
	}
	Super::OnRemoveAbility(ActorInfo, Spec);
}
