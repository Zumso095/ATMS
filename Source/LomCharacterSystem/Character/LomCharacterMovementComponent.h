// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "LomCharacterMovementComponent.generated.h"

/**
 * Character movement used by ALomCharacter.
 *
 * On the server the engine ticks the pose of remotely controlled characters only from
 * client move updates (ACharacter::UpdateMeshAllowAutonomousTickPose). Animation and
 * anim-graph physics (RigidBody, AnimDynamics) of those characters then advance in
 * irregular steps on the listen server's screen. With bServerTickRemotePoseEveryFrame
 * the mesh ticks its pose every server frame instead; root motion montages keep the
 * engine's per-move ticking.
 */
UCLASS()
class LOMCHARACTERSYSTEM_API ULomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/** Server only: tick the pose of remote clients every frame instead of on each client move. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement (Networking)")
	bool bServerTickRemotePoseEveryFrame = true;

	/**
	 * Server only: after a remote client touched a physics item it owns (e.g. a pushed box handed to it
	 * via SetOwner), accept the client's position for this many seconds instead of correcting it.
	 * The server sees client-owned items late (SmoothSync interpolation), so its capsule would hit them
	 * and rubber-band the pusher. 0 disables.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement (Networking)", meta = (ClampMin = "0", Units = "s"))
	float OwnedPushClientAuthTime = 0.5f;

	/** Server only: largest client/server position difference accepted during OwnedPushClientAuthTime. Bigger errors are corrected as usual. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement (Networking)", meta = (ClampMin = "0", Units = "cm"))
	float OwnedPushMaxClientError = 150.f;

	virtual void TickCharacterPose(float DeltaTime) override;

	virtual void ApplyImpactPhysicsForces(const FHitResult& Impact, const FVector& ImpactAcceleration, const FVector& ImpactVelocity) override;

	using Super::ServerCheckClientError;
	virtual bool ServerCheckClientError(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& ClientWorldLocation, const FVector& RelativeClientLocation, FMovementBaseInterfaceData* ClientMovementBaseInterfaceData, FName ClientBaseBoneName, uint8 ClientMovementMode) override;

	using Super::ServerMoveHandleClientError;
	virtual void ServerMoveHandleClientError(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& RelativeClientLocation, FMovementBaseInterfaceData* ClientMovementBaseInterfaceData, FName ClientBaseBoneName, uint8 ClientMovementMode) override;

	using Super::ServerShouldUseAuthoritativePosition;
	virtual bool ServerShouldUseAuthoritativePosition(float ClientTimeStamp, float DeltaTime, const FVector& Accel, const FVector& ClientWorldLocation, const FVector& RelativeClientLocation, FMovementBaseInterfaceData* ClientMovementBaseInterfaceData, FName ClientBaseBoneName, uint8 ClientMovementMode) override;

private:
	bool ShouldTickRemotePoseEveryFrame() const;

	/** Server-side copy of a character controlled by a remote client. */
	bool IsRemoteClientOnServer() const;

	/** The hit is a physics-simulated body whose actor is owned by this character. */
	bool IsOwnedPhysicsItem(const FHitResult& Impact) const;

	/** Server: the remote client recently pushed an item it owns and its position is close enough to trust. */
	bool ShouldAcceptClientPushPosition(const FVector& ClientWorldLocation) const;

	/** World time of the last contact with an owned physics item (server only). */
	double LastOwnedPushTime = -1.0;

	/** Set when ServerShouldUseAuthoritativePosition accepted the client position because of an owned push. */
	bool bAcceptedPushPositionThisMove = false;
};
