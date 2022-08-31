// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "DelveDamageType.h"
#include "HarmableInterface.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONDELVE_API ABaseEnemy : public APaperCharacter, public IHarmableInterface
{
	GENERATED_BODY()

public:
	ABaseEnemy();

protected:
	virtual void BeginPlay() override;

	void RotateSpriteTorwardsPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(USceneComponent* SpawnComp);

	UFUNCTION()
	void CombatRangeOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void CombatRangeEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex); 

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	class USceneComponent* ProjectileSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	class USphereComponent* CombatRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AProjectile> EnemyProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	class UActorAbilities* EnemyAbilities;

	class ADelveAIController* EnemyController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	FVector PatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	int Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	int Defence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bCanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	float AttackRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bTargetInRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	float StatusChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	TMap<EDamageType, int32> ResistanceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle AttackTimer;

public:

	virtual void Harmed_Implementation(FHitResult HitResult) override;

	FORCEINLINE UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;}
	FORCEINLINE ADelveAIController* GetEnemyController() const {return EnemyController;}
	FORCEINLINE bool GetCanAttack() const {return bCanAttack;}
	
};

