// Fill out your copyright notice in the Description page of Project Settings.


#include "LomCharacter.h"
#include "Components/GameFrameworkComponentManager.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"
#include "NativeGameplayTags.h"

UE_DEFINE_GAMEPLAY_TAG(Input_Block_Movement, "Input.Block.Movement");
UE_DEFINE_GAMEPLAY_TAG(Input_Block_Look, "Input.Block.Look");

// Sets default values
ALomCharacter::ALomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	abilitySystemComponent = CreateDefaultSubobject<ULomAbilitySystemComponent>(TEXT("AbilitySystem"));
	abilitySystemComponent->SetIsReplicated(true);


	HealthAttributeSet = CreateDefaultSubobject<UHealthAttributeSet>(TEXT("HealthAttributeSet"));

}

UAbilitySystemComponent* ALomCharacter::GetAbilitySystemComponent() const
{
	return abilitySystemComponent;
}

void ALomCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
	
}

// Called when the game starts or when spawned
void ALomCharacter::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
	
}

void ALomCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALomCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALomCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	/*FString netMode = "none";
	switch (GetNetMode())
	{
	case NM_Client:
		netMode = "client";
		break;
	case NM_DedicatedServer:
	case NM_ListenServer:
		netMode = "server s";
		break;


	}

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, netMode);*/
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
}

void ALomCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
}

void ALomCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if (abilitySystemComponent->HasMatchingGameplayTag(Input_Block_Movement))
	{
		//UE_LOG(LogTemp, Error, TEXT("Has tag"));
		Super::AddMovementInput(FVector::ZeroVector, 0, bForce);
		return;
	}


	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void ALomCharacter::AddControllerPitchInput(float Val)
{
	if (abilitySystemComponent->HasMatchingGameplayTag(Input_Block_Look))
	{
		//UE_LOG(LogTemp, Error, TEXT("Has tag"));
		return;
	}
	Super::AddControllerPitchInput(Val);
}

void ALomCharacter::AddControllerYawInput(float Val)
{
	if (abilitySystemComponent->HasMatchingGameplayTag(Input_Block_Look))
	{
		//UE_LOG(LogTemp, Error, TEXT("Has tag"));
		return;
	}
	Super::AddControllerYawInput(Val);
}

void ALomCharacter::AddControllerRollInput(float Val)
{
	if (abilitySystemComponent->HasMatchingGameplayTag(Input_Block_Look))
	{
		//UE_LOG(LogTemp, Error, TEXT("Has tag"));
		return;
	}
	Super::AddControllerRollInput(Val);
}

