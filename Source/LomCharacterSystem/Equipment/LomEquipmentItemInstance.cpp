// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/LomEquipmentItemInstance.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/LomAbilitySet.h"
#include "Equipment/LomEquipmentItemDefinition.h"
#include <Net/UnrealNetwork.h>
#include <Inventory/LomInventoryItemDefinition.h>
#include <Inventory/InventoryFragment_EquippableItem.h>
#include "Inventory/LomInventoryItemInstance.h"
#include "Inventory/InventoryItemFragment_UI.h"
#include <UI\LomGameUIManagerSubsystem.h>

#include UE_INLINE_GENERATED_CPP_BY_NAME(LomEquipmentItemInstance)


UWorld* ULomEquipmentItemInstance::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

AActor* ULomEquipmentItemInstance::GetOwnerActor()
{
	return  Cast<AActor>(GetOuter());
}

APawn* ULomEquipmentItemInstance::GetOwnerPawn()
{
	return  Cast<APawn>(GetOuter());

}

void ULomEquipmentItemInstance::InitInstance(ULomInventoryItemDefinition* InventoryItemDefinitionRef, ULomInventoryItemInstance* InventoryItemInstanceRef)
{
	InventoryItemDefinition = InventoryItemDefinitionRef;
	InventoryItemInstance = InventoryItemInstanceRef;
	//const ULomInventoryItemDefinition* inventoryItemDefinition = GetDefault<ULomInventoryItemDefinition>(InventoryItemDefinitionRef->GetItemDefinition());
	const UInventoryFragment_EquippableItem* equippableItem = Cast<UInventoryFragment_EquippableItem>(InventoryItemDefinition->FindFragment(UInventoryFragment_EquippableItem::StaticClass()));
	const ULomEquipmentItemDefinition* equipmentItemDefinition = GetDefault< ULomEquipmentItemDefinition>(equippableItem->EquipmentDefinition);
	AActor* owner = GetOwnerActor();
	SpawnActor(equipmentItemDefinition,owner);
	AddAbilitySet(equipmentItemDefinition->AbilitySet, owner);
	Equipped();

}

void ULomEquipmentItemInstance::InitInstanceWithParams(ULomInventoryItemDefinition* InventoryItemDefinitionRef, ULomInventoryItemInstance* InventoryItemInstanceRef, FTransform Offset)
{
	InventoryItemDefinition = InventoryItemDefinitionRef;
	InventoryItemInstance = InventoryItemInstanceRef;
	//const ULomInventoryItemDefinition* inventoryItemDefinition = GetDefault<ULomInventoryItemDefinition>(InventoryItemDefinitionRef->GetItemDefinition());
	const UInventoryFragment_EquippableItem* equippableItem = Cast<UInventoryFragment_EquippableItem>(InventoryItemDefinition->FindFragment(UInventoryFragment_EquippableItem::StaticClass()));
	const ULomEquipmentItemDefinition* equipmentItemDefinition = GetDefault< ULomEquipmentItemDefinition>(equippableItem->EquipmentDefinition);
	AActor* owner = GetOwnerActor();
	SpawnActorWithParams(equipmentItemDefinition, owner,Offset);
	AddAbilitySet(equipmentItemDefinition->AbilitySet, owner);
	Equipped();
}

void ULomEquipmentItemInstance::DeInitInstance()
{
	AActor* owner = GetOwnerActor();
	RemoveActors();
	RemoveAbilitySet(owner);
	UnEquipped();
}

void ULomEquipmentItemInstance::SpawnActor(const ULomEquipmentItemDefinition* EquipmentDefinition, AActor* owner)
{
	for (const FLomEquipmentActorPresetToSpawn& actorPreset : EquipmentDefinition->ActorsToSpawn)
	{
		AActor* spawnedActor = GetWorld()->SpawnActorDeferred<AActor>(actorPreset.ActorClass, FTransform::Identity, GetOwnerActor());
		if (spawnedActor )
		{
			//spawnedActor->SetInstigator(GetOwnerPawn());
			spawnedActor->FinishSpawning(FTransform::Identity);
			SpawnedActors.Add(spawnedActor);
			if (!actorPreset.SocketToAttach.IsNone())
			{
				TArray<USkeletalMeshComponent*> smcOuter;
				owner->GetComponents(USkeletalMeshComponent::StaticClass(), smcOuter);
				for (USkeletalMeshComponent* sm : smcOuter)
				{
					if (sm->DoesSocketExist(actorPreset.SocketToAttach))
					{
						spawnedActor->AttachToComponent(sm, FAttachmentTransformRules::KeepRelativeTransform, actorPreset.SocketToAttach);
						spawnedActor->SetActorRelativeTransform(actorPreset.Offset);
						return;
					}
				}
			}
			
			spawnedActor->AttachToActor(owner, FAttachmentTransformRules::KeepRelativeTransform, actorPreset.SocketToAttach);

			
			

		}
	}
	
	
}

void ULomEquipmentItemInstance::SpawnActorWithParams(const ULomEquipmentItemDefinition* EquipmentDefinition, AActor* owner, FTransform Transform)
{
	for (const FLomEquipmentActorPresetToSpawn& actorPreset : EquipmentDefinition->ActorsToSpawn)
	{
		AActor* spawnedActor = GetWorld()->SpawnActor(actorPreset.ActorClass);
		if (spawnedActor)
		{
			SpawnedActors.Add(spawnedActor);
			if (!actorPreset.SocketToAttach.IsNone())
			{
				TArray<USkeletalMeshComponent*> smcOuter;
				owner->GetComponents(USkeletalMeshComponent::StaticClass(), smcOuter);
				for (USkeletalMeshComponent* sm : smcOuter)
				{
					if (sm->DoesSocketExist(actorPreset.SocketToAttach))
					{


						spawnedActor->AttachToComponent(sm, FAttachmentTransformRules::KeepRelativeTransform, actorPreset.SocketToAttach);
						spawnedActor->SetActorRelativeTransform(Transform);
						return;
					}
				}
			}
			
			spawnedActor->AttachToActor(owner, FAttachmentTransformRules::KeepRelativeTransform, actorPreset.SocketToAttach);
			spawnedActor->SetActorRelativeTransform(Transform);

		}
	}
}

void ULomEquipmentItemInstance::RemoveActors()
{
	for (AActor* actorToDestroy : SpawnedActors)
	{
		if (actorToDestroy)
		{
			actorToDestroy->Destroy();
		}
		
	}
}

void ULomEquipmentItemInstance::AddAbilitySet(ULomAbilitySet* AbilitySet, AActor* Target)
{
	if (Target && AbilitySet)
	{
		UAbilitySystemComponent* abilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
		if (abilitySystem)
		{
			AbilitySet->GiveAbility(abilitySystem, this);
			attachedAbilitySet = AbilitySet;
		}
	}
}

void ULomEquipmentItemInstance::RemoveAbilitySet(AActor* Target)
{
	if (attachedAbilitySet)
	{
		UAbilitySystemComponent* abilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
		if (abilitySystem)
		{
			
			attachedAbilitySet->ClearAbility(abilitySystem);
		}
	}
}

void ULomEquipmentItemInstance::AddWidgets()
{
	APawn* ownerPawn = GetOwnerPawn();
	if (ownerPawn && ownerPawn->IsLocallyControlled() && !GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		//const ULomInventoryItemDefinition* inventoryItemDefinition = GetDefault<ULomInventoryItemDefinition>(InventoryItemDefinition->GetItemDefinition());
		const UInventoryItemFragment_UI* inventoryUIFragment = Cast<UInventoryItemFragment_UI>(InventoryItemDefinition->FindFragment(UInventoryItemFragment_UI::StaticClass()));
		if (inventoryUIFragment)
		{
			ULomGameUIManagerSubsystem* uiManager = GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>();
			for (const FUIFragmentItemContainer& widget : inventoryUIFragment->WidgetsToAdd)
			{
				if (widget.AnchorPoint.IsValid() && widget.WidgetClass)
				{
					addedWidgets.Add(uiManager->PushContentToAnchor(widget.AnchorPoint, widget.WidgetClass));
				}
			}
			/*for (ULomInventoryItemFragment* fragment : inventoryItemDefinition->Fragments)
			{
				if (Cast<UInventoryItemFragment_UI>(fragment))
				{
					for (const FUIFragmentItemContainer& widget : Cast<UInventoryItemFragment_UI>(fragment)->WidgetsToAdd)
					{
						if (widget.AnchorPoint.IsValid() && widget.WidgetClass)
						{
							addedWidgets.Add(uiManager->PushContentToAnchor(widget.AnchorPoint, widget.WidgetClass));
							GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("add widget"));

						}
					}
				}
			}*/
		}
	}

}

void ULomEquipmentItemInstance::RemoveAddedWidgets()
{
	APawn* ownerPawn = GetOwnerPawn();
	if (ownerPawn && ownerPawn->IsLocallyControlled() && !GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		ULomGameUIManagerSubsystem* uiManager = GetWorld()->GetSubsystem<ULomGameUIManagerSubsystem>();

		//ULomGameUIManagerSubsystem* uiManager = ULomGameUIManagerSubsystem::Get();
		for (UUserWidget* widget : addedWidgets)
		{
			if (widget)
			{
				uiManager->RemoveWidget(widget);
			}
		}

	}
}

void ULomEquipmentItemInstance::Equipped()
{
	//UE_LOG(LogTemp, Error, TEXT("equipped item"));
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("equipped item"));
	AActor* owner = GetOwnerActor();
	/*if (owner->IsNetMode(NM_ListenServer))
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("listen server"));

	}
	if (owner->IsNetMode(NM_DedicatedServer))
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("dedicated  server"));

	}
	if (owner->IsNetMode(NM_Client))
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, TEXT("client"));

	}*/
	if (InventoryItemDefinition)
	{
		AddWidgets();

	}

	K2_Equipped();
}

void ULomEquipmentItemInstance::UnEquipped()
{
	//UE_LOG(LogTemp, Error, TEXT("Unnnnequipped item"));
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("Uneeequipped item"));
	//GetWorld()->IsNetMode(nm)
	if (InventoryItemDefinition)
	{
		RemoveAddedWidgets();

	}
	K2_UnEquipped();
}

void ULomEquipmentItemInstance::ChangeSocket(FName SocketName, FTransform Transform, int32 ActorIndex)
{
	
	if (!SocketName.IsNone())
	{
		TArray<USkeletalMeshComponent*> smcOuter;
		GetOwnerActor()->GetComponents(USkeletalMeshComponent::StaticClass(), smcOuter);
		for (USkeletalMeshComponent* sm : smcOuter)
		{
			if (sm->DoesSocketExist(SocketName))
			{


				SpawnedActors[ActorIndex]->AttachToComponent(sm, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
				SpawnedActors[ActorIndex]->SetActorRelativeTransform(Transform);
				return;
			}
		}
	}

}

bool ULomEquipmentItemInstance::IsSupportedForNetworking() const
{
	return true;
}

void ULomEquipmentItemInstance::OnRep_InventoryItemDefinition()
{
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("OnRep_AttachedEquipmentDefinition()"));

}

void ULomEquipmentItemInstance::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(ThisClass, InventoryItemDefinition);
	DOREPLIFETIME(ThisClass, InventoryItemInstance);
	DOREPLIFETIME(ThisClass, SpawnedActors);
}
