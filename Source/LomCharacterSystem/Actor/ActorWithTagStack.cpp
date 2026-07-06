// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ActorWithTagStack.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AActorWithTagStack::AActorWithTagStack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AActorWithTagStack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorWithTagStack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 AActorWithTagStack::GetTagStackCount(FGameplayTag Tag)
{
	return TagStackContainer.GetTagStackCount(Tag);
}

void AActorWithTagStack::AddTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.AddTagStack(Tag, Count);
}

void AActorWithTagStack::OverrideTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.OverrideTagStack(Tag, Count);
}

bool AActorWithTagStack::ContainsTagStack(FGameplayTag Tag)
{
	return TagStackContainer.ContainsTagStack(Tag);
}

void AActorWithTagStack::RemoveTagStack(FGameplayTag Tag, int32 Count)
{
	TagStackContainer.RemoveTagStack(Tag, Count);
}

void AActorWithTagStack::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, TagStackContainer);
	

}