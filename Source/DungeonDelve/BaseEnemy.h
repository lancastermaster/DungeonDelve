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
	void RotateSpriteTorwardsPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

private:
	class ADelveAIController* EnemyController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxHealth; //base value should be 50 + Endurance * 10

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float AttackRate;

	//AbilityScores

	//used to calculate damage and increase carry weight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Strength;

	//increases health and carry weight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Endurance;

	//increases movement speed and jump height
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Agility;

	//increases Magic and the effectiveness of spells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Intelligence;

	//increases Magic and the Bartering skill
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Presence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	TMap<EDamageType, int32> ResistanceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bCanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle AttackTimer;

public:

	virtual void Harmed_Implementation(FHitResult HitResult) override;
	
};

