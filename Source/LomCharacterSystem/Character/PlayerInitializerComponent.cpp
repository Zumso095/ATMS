// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerInitializerComponent.h"
#include "Game/LomGameState.h"

// Sets default values for this component's properties
UPlayerInitializerComponent::UPlayerInitializerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;

	// ...
}

void UPlayerInitializerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	PawnOwner = Cast<APawn>(GetOwner());
	ensure(PawnOwner != nullptr);
}


// Called when the game starts
void UPlayerInitializerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		if (const auto AnvioGameState = Cast<ALomGameState>(GetWorld()->GetGameState()))
		{
			TriggerPlayerEnterEvent();
		}
		else
		{
			GetWorld()->GameStateSetEvent.AddUObject(this, &UPlayerInitializerComponent::OnGameStateSet);
		}
	}
	
}

void UPlayerInitializerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TriggerPlayerExitEvent();

	if (GetWorld())
	{
		GetWorld()->GameStateSetEvent.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void UPlayerInitializerComponent::OnGameStateSet(class AGameStateBase* GS)
{
	if (GS)
	{
		if (GetWorld())
		{
			GetWorld()->GameStateSetEvent.RemoveAll(this);
		}

		TriggerPlayerEnterEvent();
	}
}

void UPlayerInitializerComponent::TriggerPlayerEnterEvent()
{
	if (GetWorld())
	{
		if (const auto LomGameState = Cast<ALomGameState>(GetWorld()->GetGameState()))
		{
			LomGameState->OnPlayerEnteredGame.Broadcast(PawnOwner);
		}
	}
}

void UPlayerInitializerComponent::TriggerPlayerExitEvent()
{
	if (GetWorld())
	{
		if (const auto LomGameState = Cast<ALomGameState>(GetWorld()->GetGameState()))
		{
			LomGameState->OnPlayerLeftGame.Broadcast(PawnOwner);
		}
	}
}

// Called every frame
void UPlayerInitializerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

