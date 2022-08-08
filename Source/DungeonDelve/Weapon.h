// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DelveDamageType.h"
#include "EquipmentSlot.h"
#include "Weapon.generated.h"

/**
 * 
 */

UCLASS()
class DUNGEONDELVE_API AWeapon : public AItem
{
	GENERATED_BODY()
	
	public:

		UFUNCTION(BlueprintCallable)
		void PlayAttackSound(FVector SoundLocation);

		UFUNCTION(BlueprintCallable)
		void SpawnAttackParticles(UParticleSystem* Particles, FVector SpawnLocation, FRotator SpawnRotation);

		UFUNCTION(BlueprintImplementableEvent)
		void Attack();
		
	protected:
		AController* GetOwnerController() const;

		bool TraceUnderCrosshairs(float TraceRange, FHitResult& OutHitResult, FVector& OutHitLocation);
		
		UFUNCTION(BlueprintCallable)
		void SpawnProjectile(USceneComponent* ProjectileSpawn); 

		UFUNCTION(BlueprintCallable)
		void ShootRaycast(USceneComponent* TraceStart, TSubclassOf<class UDamageType> RaycastDamageType);

	private:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (AllowPrivateAccess = true))
		class USoundBase* AttackSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (AllowPrivateAccess = true))
		class USoundBase* HitSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		class USceneComponent* AttackOrigin;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = true))
		class UParticleSystem* AttackParticles;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TSubclassOf<class AProjectile> WeaponProjectile;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		EDamageType WeaponDamageType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		TSubclassOf<class UDamageType> DamageType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		int Damage;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		float AttackRate;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		float WeaponRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
		bool bTwoHands = false;

	public:
	//for the getters and setters
		FORCEINLINE int GetDamage() const {return Damage;}
		FORCEINLINE float GetAttackRate() const {return AttackRate;}
		FORCEINLINE float GetWeaponRange() const {return WeaponRange;}
		FORCEINLINE bool GetTwoHands() const {return bTwoHands;}
		FORCEINLINE EDamageType GetWeaponDamageType() const {return WeaponDamageType;}
};
