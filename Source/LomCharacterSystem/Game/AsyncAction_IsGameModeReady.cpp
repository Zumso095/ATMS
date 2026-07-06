// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AsyncAction_IsGameModeReady.h"
#include "LomGameModeManagerComponent.h"

UAsyncAction_IsGameModeReady* UAsyncAction_IsGameModeReady::IsGameModeReady(UObject* WorldContextObject)
{
    UAsyncAction_IsGameModeReady* action = NewObject< UAsyncAction_IsGameModeReady>();
    action->worldContextObject = WorldContextObject;
    action->RegisterWithGameInstance(WorldContextObject);
    return action;
}

void UAsyncAction_IsGameModeReady::Activate()
{
    Super::Activate();
  //  UE_LOG(LogTemp, Error, TEXT("AsyncAction: Activated"));
    UWorld* world = GEngine->GetWorldFromContextObject(worldContextObject,EGetWorldErrorMode::ReturnNull);
    AGameStateBase* gameState = world->GetGameState();
    ULomGameModeManagerComponent* gameModeManager = gameState->FindComponentByClass<ULomGameModeManagerComponent>();
    if (gameModeManager->IsGameModeDefReady)
    {
      //  UE_LOG(LogTemp, Error, TEXT("AsyncAction: game mode already ready"));

        Finish();
    }
    else
    {
       // UE_LOG(LogTemp, Error, TEXT("AsyncAction: try to bind"));

        gameModeManager->OnGameModeDefinitionReady.AddUObject(this, &UAsyncAction_IsGameModeReady::Finish);
    }


       // SetReadyToDestroy();
}

void UAsyncAction_IsGameModeReady::Finish()
{
    //UE_LOG(LogTemp, Error, TEXT("AsyncAction: finish"));

    Ready.Broadcast();
    SetReadyToDestroy();

}
