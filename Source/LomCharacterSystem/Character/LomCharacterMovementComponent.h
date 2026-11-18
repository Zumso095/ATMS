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

	virtual void TickCharacterPose(float DeltaTime) override;

private:
	bool ShouldTickRemotePoseEveryFrame() const;
};
