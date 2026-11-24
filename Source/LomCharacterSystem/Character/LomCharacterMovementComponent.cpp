// Fill out your copyright notice in the Description page of Project Settings.

#include "LomCharacterMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"

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
