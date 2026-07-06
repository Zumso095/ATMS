// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamSubsystem.h"

void UTeamSubsystem::SetTeam(AActor* actor, int32 team)
{
	if (!actor) {
		return;
	}

	Teams.Add(actor, team);
}

ELomTeamComparison UTeamSubsystem::CompareTeam(AActor* actorA, AActor* actorB)
{
	if (!actorA || !actorB) {
		return ELomTeamComparison::Wrong;
	}
	if (Teams.Contains(actorA) && Teams.Contains(actorB)) {	
		if (*Teams.Find(actorA) == *Teams.Find(actorB)) {
			return ELomTeamComparison::TheSame;
		}
		else {
			return ELomTeamComparison::Different;
		}
	}
	else {
		return ELomTeamComparison::Wrong;
	}
	
}
