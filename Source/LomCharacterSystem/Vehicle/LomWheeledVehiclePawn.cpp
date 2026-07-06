// Fill out your copyright notice in the Description page of Project Settings.


#include "LomWheeledVehiclePawn.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Game/LomGameModeManagerComponent.h"

ALomWheeledVehiclePawn::ALomWheeledVehiclePawn()
{
	abilitySystemComponent = CreateDefaultSubobject<ULomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	abilitySystemComponent->SetIsReplicated(true);

	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));
}

void ALomWheeledVehiclePawn::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();

	// A temporarely method for waiting while actor will be ready for binding input mapping
	/*ULomGameModeManagerComponent* gameModeManager = GetWorld()->GetGameState()->FindComponentByClass<ULomGameModeManagerComponent>();
	if (gameModeManager)
	{
		if (gameModeManager->IsGameModeDefReady)
		{
			ReadyForInput();
		}
		else
		{
			gameModeManager->OnGameModeDefinitionReady.AddUniqueDynamic(this, &ALomWheeledVehiclePawn::ReadyForInput);
		}
	}*/
}

void ALomWheeledVehiclePawn::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}
void ALomWheeledVehiclePawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

}

void ALomWheeledVehiclePawn::EndPlay(EEndPlayReason::Type EndPlayreason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayreason);
}

void ALomWheeledVehiclePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	/*ULomGameModeManagerComponent* gameModeManager = GetWorld()->GetGameState()->FindComponentByClass<ULomGameModeManagerComponent>();
	if (gameModeManager)
	{
		if (gameModeManager->IsGameModeDefReady)
		{
			ReadyForInput();
		}
		else
		{
			gameModeManager->OnGameModeDefinitionReady.AddUniqueDynamic(this, &ALomWheeledVehiclePawn::ReadyForInput);
		}
	}*/
}

UAbilitySystemComponent* ALomWheeledVehiclePawn::GetAbilitySystemComponent() const
{
	return abilitySystemComponent;
}

void ALomWheeledVehiclePawn::ReadyForInput()
{
	//UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, FName(TEXT("ReadyForInput")));

}
