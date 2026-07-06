// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"
#include "IPoolable.h"

DECLARE_LOG_CATEGORY_CLASS(ObjectPoolLog, All, All)

//void UObjectPoolSubsystem::Pop()
//{
//}


//AActor* UObjectPoolSubsystem::SpawnActorObjectPool(TSubclassOf<AActor> actorClass, FTransform actorTransform, AActor* owner, APawn* instigator)
//{
//	if (UObjectPoolSubsystem::Get(owner))
//	{
//		return UObjectPoolSubsystem::Get(owner)->SpawnOrPop(actorClass, actorTransform, owner, instigator);
//
//	}
//	return nullptr;
//}

AActor* UObjectPoolSubsystem::SpawnOrPop(TSubclassOf<AActor> actorClass, FTransform actorTransform, AActor* owner, APawn* instigator)
{
	if (!actorClass || !actorClass->ImplementsInterface(UIPoolable::StaticClass()))
	{
		return nullptr;
	}

	//UE_LOG(ObjectPoolLog, Display, TEXT("spawnOrPop"));


	TArray<AActor*>& foundedVales = PoolList.FindOrAdd(actorClass).Actors;
		
			if (foundedVales.Num() > 0)
			{
				for (auto object : foundedVales)
				{
					//UE_LOG(ObjectPoolLog, Error, TEXT("for: %s "), *object->GetName());

					if (object && !IIPoolable::Execute_IsActiveObject(object))
					{
						//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, object->GetName());
						
						object->SetOwner(owner);
						object->SetInstigator(instigator);
						object->SetActorTransform(actorTransform, false, nullptr, ETeleportType::TeleportPhysics);
						IIPoolable::Execute_ActivateObject(object);
						UE_LOG(ObjectPoolLog, Display, TEXT("reusing object: %s "),*object->GetName() );

						return object;
					}
				
				}
			}
			// spawn new object because of there is no free object in pool

			FActorSpawnParameters spawnParameters;
			spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			spawnParameters.Owner = owner;
			spawnParameters.Instigator = instigator;

			AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(actorClass, actorTransform, spawnParameters);
			if (spawnedActor)
			{
				
				foundedVales.Add(spawnedActor);
				//PoolList.Add(actorClass, *foundedVales);
				IIPoolable::Execute_ActivateObject(spawnedActor);
				//UE_LOG(ObjectPoolLog, Display, TEXT("spawned new object because not found"));

				return spawnedActor;
			}


			return nullptr;
	
}

void UObjectPoolSubsystem::ReserveActors(TSubclassOf<AActor> actorClass, int32 count)
{
	if (!actorClass || !actorClass->ImplementsInterface(UIPoolable::StaticClass()))
	{
		return;
	}

	TArray<AActor*> newArray = PoolList.FindOrAdd(actorClass).Actors;
	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int32 index = 0; index < count; index++)
	{
		
		
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(actorClass,spawnParameters); // needs deffered spawn?
		if (spawnedActor)
		{
				IIPoolable::Execute_DeactivateObject(spawnedActor);
				newArray.Add(spawnedActor);
		}

	}
	//PoolList.Add(actorClass, newArray);
}

void UObjectPoolSubsystem::ShowObjectPoolDebug()
{
	//FString text = );
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, text);
	/*for (TPair<TSubclassOf<AActor>, FPoolActorArray> poolPair : PoolList)
	{
		UE_LOG(ObjectPoolLog, Display, TEXT("object pool actor class: %s = %s actors"),*poolPair.Key->GetDisplayNameText().ToString(), *FString::FromInt(poolPair.Value.Actors.Num()));

	}*/
}



