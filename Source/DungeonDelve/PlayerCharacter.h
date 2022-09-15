// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DelveDamageType.h"
#include "EquipmentSlot.h"
#include "GameFramework/Character.h"
#include "HarmableInterface.h"
#include "Item.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DUNGEONDELVE_API APlayerCharacter : public ACharacter, public IHarmableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookRight(float AxisValue);

	void PrimaryAction();
	void SecondaryAction();

	void LeftClickDown();
	void LeftClickUp();
	void RightClickDown();
	void RightClickUp();
	
	void Interact();

	UFUNCTION(BlueprintImplementableEvent)
	void CallInteract(AActor* InteractActor);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleCharacterSheet();

	UFUNCTION(BlueprintImplementableEvent)
	void SetInventoryItemReference(AItem* Item);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleAbility(AAbility* Ability);

	void InitializeDerivedStats();

	UFUNCTION(BlueprintCallable)
	bool TraceUnderCrosshairs(float TraceRange, FHitResult& OutHitResult, FVector& OutHitLocation);

	void TraceForItems();

	void TraceForInteractables();

	UFUNCTION(BlueprintCallable)
	void EquipItem(EEquipmentSlot EquipmentSlot, AItem* ItemToEquip);

	void PickupItem(AItem* ItemToPickup);

	void SpawnDefaultWeapon();

	void StartAttackTimer();
	void StartSecondaryTimer();

	void ResetCanAttack();
	void ResetSecondaryReady();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void DropItem(AItem* ItemToDrop);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	class USceneComponent* MainHandSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	class UActorAbilities* PlayerAbilities;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float RotationRate = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float InteractReach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxHealth; //base value should be 50 + (Endurance * 10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxMagic; //base value should be 50 + (Intelligence * 10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int DamageBoost; //add Strength

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Defence;

	//AbilityScores

	//used to calculate damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Strength;

	//increases health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Endurance;

	//increases Defence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Agility;

	//increases Magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Intelligence;

	//increases Magic, Fire, Lightning, and Cold resists
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int Presence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	TMap<EDamageType, int32> ResistanceMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	bool bTraceForItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	bool bInteractTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bPrimaryDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bSecondaryDown;

	int8 OverlappedItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	class AItem* TraceHitItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	AItem* TraceHitItemLastFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = true))
	AActor* TraceHitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	TSubclassOf<AItem>DefaultWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AItem*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TMap<EEquipmentSlot, AItem*> EquippedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bCanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bSecondaryReady;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle AttackTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle SecondaryAttackTimer;
	
public:	
	FORCEINLINE int GetHealth() {return Health;}
	FORCEINLINE int GetMaxHealth() {return MaxHealth;}
	FORCEINLINE int GetMagic() {return Magic;}
	FORCEINLINE int GetMaxMagic() {return MaxMagic;}
	FORCEINLINE int GetGold() {return Gold;}
	FORCEINLINE bool GetDead() {return bDead;}
	FORCEINLINE bool GetInteractTrace() {return bInteractTrace;}
	FORCEINLINE bool GetItemTrace() {return bTraceForItems;}

	void SetGold(int Value);
	void SetInteractTrace(bool Trace);
	void SetItemTrace(bool Trace);

	virtual void Harmed_Implementation(FHitResult HitResult) override;
};
