// Fill out your copyright notice in the Description page of Project Settings.


#include "LomGameMode.h"
#include "LomWorldSettings.h"
#include "LomGameModeDefinition.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h"
#include "LomGameState.h"
#include "Player/LomPlayerController.h"
#include "Player/LomPlayerState.h"
#include "UI/LomHUD.h"
#include "LomGameModeManagerComponent.h"
#include <AbilitySystemGlobals.h>


ALomGameMode::ALomGameMode()
{
	GameStateClass = ALomGameState::StaticClass();
	PlayerControllerClass = ALomPlayerController::StaticClass();
	PlayerStateClass = ALomPlayerState::StaticClass();
	HUDClass = ALomHUD::StaticClass();
}

void ALomGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	GetWorldTimerManager().SetTimerForNextTick(this, &ALomGameMode::InitGameModeManager);
	//InitGameModeManager();
}

void ALomGameMode::InitGameModeManager()
{
	//UE_LOG(LogTemp, Error, TEXT("InitGameMode"));

	if (ALomGameState* gameState = GetGameState<ALomGameState>())
	{
		//UE_LOG(LogTemp, Error, TEXT("GetGameState"));

		if (ULomGameModeManagerComponent* gameModeManager = gameState->FindComponentByClass<ULomGameModeManagerComponent>())
		{
			//UE_LOG(LogTemp, Error, TEXT("Init"));
			gameModeManager->OnGameModeDefinitionReady.AddUObject(this, &ALomGameMode::IsGameModeReady);

			gameModeManager->Init();
		}
	}

}

void ALomGameMode::SpawnActorFromPawnData(ULomPawnData* PawnData, FTransform SpawnTransform)
{

	if (PawnData)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = GetInstigator();
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
		UClass* PawnClass = PawnData->DefaultPawn;
		APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
		if (!ResultPawn)
		{
			UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}

		if (ResultPawn)
		{
			UAbilitySystemComponent* abilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ResultPawn);
			if (abilitySystem)
			{
				for (ULomAbilitySet* abilitySet : PawnData->AbilitySets)
				{

					if (abilitySet)
					{
						abilitySet->GiveAbility(abilitySystem, nullptr);

					}
					else
					{
						UE_LOG(LogGameMode, Error, TEXT("AbilitySet is missed"));

					}

				}
			}
		}
	}

}

void ALomGameMode::SetGameModeDefinition()
{
	//UE_LOG(LogTemp, Error, TEXT("SetGameModeDefinition"));
	ALomWorldSettings* worldSettigns = Cast<ALomWorldSettings>(GetWorldSettings());
	if (!worldSettigns)
		return;
	
	if (worldSettigns->GetDefaultGameModeDefinition())
	{
		/*FString na = worldSettigns->GetDefaultGameModeDefinition()->name;

		UE_LOG(LogTemp, Error, TEXT("game mode definition name = %s"),*na);*/

	}

	//UGameFeatureAction* action = worldSettigns->GetDefaultGameModeDefinition()->TestAction;
	//if (action)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("trying activate action"));

	//	FGameFeatureActivatingContext Context;

	//	// Only apply to our specific world context if set
	//	const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
	//	if (ExistingWorldContext)
	//	{
	//		Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
	//	}
	//	action->OnGameFeatureRegistering();
	//	action->OnGameFeatureLoading();
	//	action->OnGameFeatureActivating(Context);
	//}
}

UClass* ALomGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (ALomGameState* gameState = GetGameState<ALomGameState>())
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), (ULomGameModeManagerComponent::IsGameModeDefReady ? TEXT("true") : TEXT("false")));
		if (ULomGameModeManagerComponent* gameModeManager = gameState->FindComponentByClass<ULomGameModeManagerComponent>())
		{
			if (gameModeManager->IsGameModeDefReady && gameModeManager->CurrentGameModeDefinition->PawnData && gameModeManager->CurrentGameModeDefinition->PawnData->DefaultPawn)
			{
				
					UE_LOG(LogTemp, Error, TEXT("game mode definition found"));
					return gameModeManager->CurrentGameModeDefinition->PawnData->DefaultPawn;
			}
			else
			{
					UE_LOG(LogTemp, Error, TEXT("game mode definition not ready"));
			}
		}
		

		
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ALomGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	//if (ALomGameState* gameState = GetGameState<ALomGameState>())
	//{
	//	//UE_LOG(LogTemp, Error, TEXT("%s"), (ULomGameModeManagerComponent::IsGameModeDefReady ? TEXT("true") : TEXT("false")));
	//	if (ULomGameModeManagerComponent* gameModeManager = gameState->FindComponentByClass<ULomGameModeManagerComponent>())
	//	{
	//		if (gameModeManager->IsGameModeDefReady && gameModeManager->CurrentGameModeDefinition->PawnData && gameModeManager->CurrentGameModeDefinition->PawnData->DefaultPawn)
	//		{

	//			UE_LOG(LogTemp, Error, TEXT("game mode definition found"));
	//			
	//		}
	//		
	//	}



	//}
	//UE_LOG(LogTemp, Error, TEXT("spawn can't find gamedefinition"));

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);
	APawn* ResultPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo);
	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}

	if (ALomGameState* gameState = GetGameState<ALomGameState>())
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), (ULomGameModeManagerComponent::IsGameModeDefReady ? TEXT("true") : TEXT("false")));
		if (ULomGameModeManagerComponent* gameModeManager = gameState->FindComponentByClass<ULomGameModeManagerComponent>())
		{
			if (gameModeManager->IsGameModeDefReady && gameModeManager->CurrentGameModeDefinition->PawnData)
			{

				//UE_LOG(LogTemp, Error, TEXT("game mode definition found"));
				
				UAbilitySystemComponent* abilitySystem = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ResultPawn);
				if (abilitySystem)
				{
					for (ULomAbilitySet* abilitySet : gameModeManager->CurrentGameModeDefinition->PawnData->AbilitySets)
					{
					
						if (abilitySet)
						{
							abilitySet->GiveAbility(abilitySystem, nullptr);

						}
						else
						{
							UE_LOG(LogGameMode, Error, TEXT("AbilitySet is missed"));

						}
						
					}
				}

			}
			
		}



	}

	return ResultPawn;
}

void ALomGameMode::IsGameModeReady()
{
	//UE_LOG(LogTemp, Error, TEXT("now we can dispatch"));
	//UE_LOG(LogTemp, Error, TEXT("%s"),*GetWorld()->GetFirstPlayerController()->GetName());

	//DispatchPostLogin(GetWorld()->GetFirstPlayerController());
	//GetWorld()->GetFirstPlayerController()->GetPawn()->Destroy();
	//GetWorld()->GetFirstPlayerController()->UnPossess();

	//RestartPlayer(GetWorld()->GetFirstPlayerController());

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				UE_LOG(LogGameMode, Warning, TEXT("Trying to restart player"));

				RestartPlayer(PC);
			}
		}
	}
}
