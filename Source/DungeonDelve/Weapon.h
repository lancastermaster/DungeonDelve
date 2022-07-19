// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DelveDamageType.h"
#include "Weapon.generated.h"

/**
 * 
 */

UCLASS()
class DUNGEONDELVE_API AWeapon : public AItem
{
	GENERATED_BODY()
	
	public:

	protected:
		AController* GetOwnerController() const;

	private:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (AllowPrivateAccess = true))
		USoundBase* AttackSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		EDamageType WeaponDamageType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		int Damage;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		float AttackRate;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		bool bRanged = false;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		bool bTwoHands = false;

	public:
	//for the getters and setters
		FORCEINLINE int GetDamage() const {return Damage;}
		FORCEINLINE float GetAttackRate() const {return AttackRate;}
		FORCEINLINE bool GetIsRanged() const {return bRanged;}
		FORCEINLINE bool GetTwoHands() const {return bTwoHands;}
		FORCEINLINE EDamageType GetWeaponDamageType() const {return WeaponDamageType;}
};
