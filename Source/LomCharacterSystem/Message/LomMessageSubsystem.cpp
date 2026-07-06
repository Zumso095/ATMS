// Fill out your copyright notice in the Description page of Project Settings.


#include "Message/LomMessageSubsystem.h"

void ULomMessageSubsystem::BroadcastMessageTemp(FGameplayTag Channel, UScriptStruct* Payload)
{
	OnMessageSend.Broadcast();
	if (Channel.IsValid())
	{
		if (Listeners.Contains(Channel))
		{
			for (UObject* listener : *Listeners.Find(Channel))
			{

			}
		}
	}
}
