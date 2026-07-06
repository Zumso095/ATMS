// Fill out your copyright notice in the Description page of Project Settings.


#include "LomPlayerController.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Game/LomGameModeManagerComponent.h"

const FName ALomPlayerController::NAME_ReadyForInput("ReadyForInput");


void ALomPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ALomPlayerController::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
	//UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(GetPawn(), ALomPlayerController::NAME_ReadyForInput);

}

void ALomPlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void ALomPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	//if (!HasAuthority() && IsLocalController())
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("AcknowledgePOssession == liseten  server"));
	//	// Your logic here
	//}
	if (!GetWorld() || !GetWorld()->GetGameState())
		return;
	ULomGameModeManagerComponent* gameModeManager = GetWorld()->GetGameState()->FindComponentByClass<ULomGameModeManagerComponent>();
	if (gameModeManager)
	{
		if (gameModeManager->IsGameModeDefReady)
		{
			GameModeLoadCompleted();
		}
		else
		{
			gameModeManager->OnGameModeDefinitionReady.AddUObject(this, &ALomPlayerController::GameModeLoadCompleted);
		}
	}
	
	/*switch (GetNetMode())
	{
	case NM_Standalone:
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("AcknowledgePOssession == standlone"));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, GetName());
	break;
	case NM_Client:
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("AcknowledgePOssession == client"));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, GetName());

		break;
	case NM_DedicatedServer:
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("AcknowledgePOssession == dedicated server"));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, GetName());
		break;
	case NM_ListenServer:
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, TEXT("AcknowledgePOssession == liseten  server"));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, GetName());

		break;

	}*/
	//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, ALomPlayerController::NAME_ReadyForInput.ToString());
}


void ALomPlayerController::GameModeLoadCompleted()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(GetPawn(), ALomPlayerController::NAME_ReadyForInput);

}
