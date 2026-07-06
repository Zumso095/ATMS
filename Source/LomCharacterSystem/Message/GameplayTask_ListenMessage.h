// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "GameplayTags.h"
#include "GameplayTask_ListenMessage.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMessageCallBack);

UCLASS()
class LOMCHARACTERSYSTEM_API UGameplayTask_ListenMessage : public UGameplayTask
{

	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnMessageCallBack Listen;

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category = "MessageSystem",meta = (DefaultToSelf = "TaskOwner"))
	static UGameplayTask_ListenMessage* ListenMessageTask(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, FGameplayTag Channel);

	FGameplayTag Channel;

	void MessageHandle();
};
