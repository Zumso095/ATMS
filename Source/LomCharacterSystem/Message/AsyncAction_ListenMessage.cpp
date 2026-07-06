// Fill out your copyright notice in the Description page of Project Settings.


#include "Message/AsyncAction_ListenMessage.h"

UAsyncAction_ListenMessage* UAsyncAction_ListenMessage::ListenMessageAsync(UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* Payload)
{

	UWorld* world = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (world == nullptr)
	{
		return nullptr;

	}
	UAsyncAction_ListenMessage* listener = NewObject< UAsyncAction_ListenMessage>();
	listener->world = world;
	listener->worldContextObject = WorldContextObject;
	listener->channel = Channel;
	listener->Payload = Payload;
	listener->RegisterWithGameInstance(world);
	return listener;
}

void UAsyncAction_ListenMessage::Activate()
{

	
}
