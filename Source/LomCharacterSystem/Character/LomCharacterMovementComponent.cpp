// Fill out your copyright notice in the Description page of Project Settings.

#include "LomCharacterMovementComponent.h"

#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

bool ULomCharacterMovementComponent::IsRemoteClientOnServer() const
{
	return CharacterOwner
		&& CharacterOwner->GetLocalRole() == ROLE_Authority
		&& CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy
		&& !CharacterOwner->IsLocallyControlled();
}

bool ULomCharacterMovementComponent::IsOwnedPhysicsItem(const FHitResult& Impact) const
{
	const UPrimitiveComponent* ImpactComponent = Impact.GetComponent();
	const AActor* ImpactActor = Impact.GetActor();
	if (!ImpactComponent || !ImpactActor || ImpactActor->GetOwner() != CharacterOwner)
	{
		return false;
	}

	return ImpactComponent->IsSimulatingPhysics(Impact.BoneName);
}

bool ULomCharacterMovementComponent::ShouldAcceptClientPushPosition(const FVector& ClientWorldLocation) const
{
	if (OwnedPushClientAuthTime <= 0.f || LastOwnedPushTime < 0.0 || !UpdatedComponent || !IsRemoteClientOnServer())
	{
		return false;
	}

	const UWorld* World = GetWorld();
	if (!World || World->GetTimeSeconds() - LastOwnedPushTime > OwnedPushClientAuthTime)
	{
		return false;
	}

	return FVector::DistSquared(ClientWorldLocation, UpdatedComponent->GetComponentLocation()) <= FMath::Square(OwnedPushMaxClientError);
}

void ULomCharacterMovementComponent::ApplyImpactPhysicsForces(const FHitResult& Impact, const FVector& ImpactAcceleration, const FVector& ImpactVelocity)
{
	if (bEnablePhysicsInteraction && Impact.bBlockingHit && IsRemoteClientOnServer() && IsOwnedPhysicsItem(Impact))
	{
		if (const UWorld* World = GetWorld())
		{
			LastOwnedPushTime = World->GetTimeSeconds();
		}

		// The owning client simulates and pushes this item; the server copy follows its replicated transform.
		return;
	}

	Super::ApplyImpactPhysicsForces(Impact, ImpactAcceleration, ImpactVelocity);
}

bool ULomCharacterMovementComponent::ServerCheckClientError(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& ClientWorldLocation, const FVector& RelativeClientLocation, FMovementBaseInterfaceData* ClientMovementBaseInterfaceData, FName ClientBaseBoneName, uint8 ClientMovementMode)
{
	if (ShouldAcceptClientPushPosition(ClientWorldLocation))
	{
		return false;
	}

	return Super::ServerCheckClientError(ClientTimeStamp, DeltaTime, Accel, ClientWorldLocation, RelativeClientLocation, ClientMovementBaseInterfaceData, ClientBaseBoneName, ClientMovementMode);
}

void ULomCharacterMovementComponent::ServerMoveHandleClientError(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& RelativeClientLocation, FMovementBaseInterfaceData* ClientMovementBaseInterfaceData, FName ClientBaseBoneName, uint8 ClientMovementMode)
{
	bAcceptedPushPositionThisMove = false;
	const FVector OldLocation = UpdatedComponent ? UpdatedComponent->GetComponentLocation() : FVector::ZeroVector;
	const FQuat OldRotation = UpdatedComponent ? UpdatedComponent->GetComponentQuat() : FQuat::Identity;

	Super::ServerMoveHandleClientError(ClientTimeStamp, DeltaTime, Accel, RelativeClientLocation, ClientMovementBaseInterfaceData, ClientBaseBoneName, ClientMovementMode);

	// The engine snaps the capsule to the accepted client position without smoothing.
	// On a listen server, smooth the mesh over that snap like a regular remote client move.
	if (bAcceptedPushPositionThisMove && UpdatedComponent && CharacterOwner
		&& IsNetMode(NM_ListenServer) && CharacterOwner->GetRemoteRole() == ROLE_AutonomousProxy)
	{
		const FVector NewLocation = UpdatedComponent->GetComponentLocation();
		if (!NewLocation.Equals(OldLocation))
		{
			SmoothCorrection(OldLocation, OldRotation, NewLocation, UpdatedComponent->GetComponentQuat());
		}
	}

	bAcceptedPushPositionThisMove = false;
}

bool ULomCharacterMovementComponent::ServerShouldUseAuthoritativePosition(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& ClientWorldLocation, const FVector& RelativeClientLocation, FMovementBaseInterfaceData* ClientMovementBaseInterfaceData, FName ClientBaseBoneName, uint8 ClientMovementMode)
{
	if (ShouldAcceptClientPushPosition(ClientWorldLocation))
	{
		bAcceptedPushPositionThisMove = true;
		return true;
	}

	return Super::ServerShouldUseAuthoritativePosition(ClientTimeStamp, DeltaTime, Accel, ClientWorldLocation, RelativeClientLocation, ClientMovementBaseInterfaceData, ClientBaseBoneName, ClientMovementMode);
}

bool ULomCharacterMovementComponent::ShouldTickRemotePoseEveryFrame() const
{
	if (!bServerTickRemotePoseEveryFrame || !CharacterOwner || !CharacterOwner->GetMesh())
	{
		return false;
	}

	// Server-side copy of a character controlled by a remote client.
	if (CharacterOwner->GetLocalRole() != ROLE_Authority
		|| CharacterOwner->GetRemoteRole() != ROLE_AutonomousProxy
		|| CharacterOwner->IsLocallyControlled())
	{
		return false;
	}

	// Root motion has to stay in sync with client moves.
	return !CharacterOwner->IsPlayingNetworkedRootMotionMontage();
}

void ULomCharacterMovementComponent::TickCharacterPose(float DeltaTime)
{
	if (ShouldTickRemotePoseEveryFrame())
	{
		// Skip the per-move tick and let the mesh tick its own pose every frame.
		// The engine sets the flag again on possession changes, so clear it on every move.
		CharacterOwner->GetMesh()->bOnlyAllowAutonomousTickPose = false;
		return;
	}

	// Engine behaviour: restore the flag (e.g. a root motion montage started) and tick per move.
	if (CharacterOwner && CharacterOwner->GetMesh())
	{
		CharacterOwner->UpdateMeshAllowAutonomousTickPose();
	}

	Super::TickCharacterPose(DeltaTime);
}
