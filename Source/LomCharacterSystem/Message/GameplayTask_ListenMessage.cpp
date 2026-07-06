// Fill out your copyright notice in the Description page of Project Settings.


#include "Message/GameplayTask_ListenMessage.h"
#include "LomMessageSubsystem.h"

void UGameplayTask_ListenMessage::Activate()
{
	ULomMessageSubsystem* messageSubsystem = GetWorld()->GetSubsystem<ULomMessageSubsystem>();
		if (messageSubsystem)
		{
			messageSubsystem->OnMessageSend.AddUObject(this, &UGameplayTask_ListenMessage::MessageHandle);
		}
}

UGameplayTask_ListenMessage* UGameplayTask_ListenMessage::ListenMessageTask(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, FGameplayTag Channel)
{
	UGameplayTask_ListenMessage* listenTask = NewTask<UGameplayTask_ListenMessage>(*TaskOwner);
	listenTask->Channel = Channel;
	listenTask->InitTask(*TaskOwner, 0);
	return listenTask;
}

void UGameplayTask_ListenMessage::MessageHandle()
{
	Listen.Broadcast();
}
