// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Attribute/LomAttributeSet.h"
#include "AbilitySystem/LomAbilitySystemComponent.h"


#include "GunRAttributeSet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GUNROULETTERUNTIME_API UGunRAttributeSet : public ULomAttributeSet
{
	GENERATED_BODY()
public:

	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, HealthCoefficient);
	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, WalkSpeed);
	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, WalkSpeedBase);
	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, FireRate);
	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, BulletSpeed);
	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, MagazineSize);
	ATTRIBUTE_ACCESSORS(UGunRAttributeSet, Stamina);
	
protected:

	UFUNCTION()
	void OnRep_HealthCoefficient(FGameplayAttributeData& value);

	UFUNCTION()
	void OnRep_WalkSpeed(FGameplayAttributeData& value);

	UFUNCTION()
	void OnRep_WalkSpeedBase(FGameplayAttributeData& value);

	UFUNCTION()
	void OnRep_Damage(FGameplayAttributeData& value);

	UFUNCTION()
	void OnRep_FireRate(FGameplayAttributeData& value);

	UFUNCTION()
	void OnRep_BulletSpeed(FGameplayAttributeData& value);

	UFUNCTION()
	void OnRep_MagazineSize(FGameplayAttributeData& value);

	UFUNCTION()
	void OnRep_Stamina(FGameplayAttributeData& value);

	
private:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthCoefficient, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData HealthCoefficient;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WalkSpeed, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WalkSpeed;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WalkSpeedBase, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData WalkSpeedBase;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Damage, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireRate, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData FireRate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BulletSpeed, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BulletSpeed;


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagazineSize, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MagazineSize;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

};
