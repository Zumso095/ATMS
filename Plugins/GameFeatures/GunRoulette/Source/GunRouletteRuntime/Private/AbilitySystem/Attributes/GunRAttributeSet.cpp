// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/GunRAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"


void UGunRAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, HealthCoefficient, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, WalkSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, WalkSpeedBase, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, FireRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, BulletSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, MagazineSize, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGunRAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	

}

void UGunRAttributeSet::OnRep_HealthCoefficient(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, HealthCoefficient, value);

}

void UGunRAttributeSet::OnRep_WalkSpeed(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, WalkSpeed, value);

}

void UGunRAttributeSet::OnRep_WalkSpeedBase(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, WalkSpeedBase, value);

}

void UGunRAttributeSet::OnRep_Damage(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, Damage, value);

}

void UGunRAttributeSet::OnRep_FireRate(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, FireRate, value);

}

void UGunRAttributeSet::OnRep_BulletSpeed(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, BulletSpeed, value);

}

void UGunRAttributeSet::OnRep_MagazineSize(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, MagazineSize, value);

}

void UGunRAttributeSet::OnRep_Stamina(FGameplayAttributeData& value)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGunRAttributeSet, Stamina, value);

}
