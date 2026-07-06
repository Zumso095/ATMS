// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LomWeaponStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "LomAimTarget.h"
#include "Actor/LomHealthComponent.h"
// Sets default values for this component's properties
ULomWeaponStateManager::ULomWeaponStateManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	AngleDot = 0.94;
	// ...
}


void ULomWeaponStateManager::FindTarget()
{
	double nearestDistance = 100000000;
	TArray<AActor*> foundActors;
	AActor* mostRelevantTarget = nullptr;
	FVector aimTargetLocation;
	FVector aimOwnerLocation = GetAimTargetLocation(GetOwner());
	FVector direction;
	UGameplayStatics::GetAllActorsOfClass(this, APawn::StaticClass(), foundActors);
	for (AActor* actor : foundActors)
	{
		if (actor != GetOwner())
		{
			aimTargetLocation = GetAimTargetLocation(actor);

			double dot = FVector::DotProduct((aimTargetLocation - aimOwnerLocation).GetSafeNormal(), GetOwner()->GetActorForwardVector());
			if (AngleDot < dot)
			{
				FHitResult hitResult;
				FCollisionQueryParams queryParams;
				queryParams.AddIgnoredActor(GetOwner());
				bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, aimOwnerLocation, aimTargetLocation, ECC_Visibility, queryParams);
				if (bHit)
				{
					if (hitResult.GetActor() == actor && actor->FindComponentByClass<ULomHealthComponent>()->IsAlive)
					{
						double distance = FVector::Distance(aimOwnerLocation, aimTargetLocation);
						if (distance < nearestDistance)
						{
							mostRelevantTarget = actor;
							nearestDistance = distance;
							direction = (aimTargetLocation - aimOwnerLocation).GetSafeNormal();
						}
						
						//UE_LOG(LogTemp, Error, TEXT(""))

						//return;
					}
					
				}
			}
		}
		//GetOwner()
	}
	if (currentTarget != mostRelevantTarget)
	{
		currentTarget = mostRelevantTarget;
		OnTargetFound.Broadcast(currentTarget,direction);
	}
	/*currentTarget = nullptr;
	OnTargetFound.Broadcast(currentTarget);*/

	
}

FVector ULomWeaponStateManager::GetAimTargetLocation(AActor* target)
{
	if (ULomAimTarget* aimTarget = target->FindComponentByClass<ULomAimTarget>())
	{
		return aimTarget->GetComponentLocation();
	}
	return target->GetActorLocation();
}

// Called when the game starts
void ULomWeaponStateManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle,this,&ULomWeaponStateManager::FindTarget, .5f, true);
	
}


// Called every frame
void ULomWeaponStateManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

