// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DelveDamageType.h"
#include "EquipmentSlot.h"
#include "GameFramework/Character.h"
#include "HarmableInterface.h"
#include "Item.h"
#include "PlayerCharacter.generated.h"

USTRUCT(BlueprintType) //Don't use this for everything - use for saving
struct FPlayerInfo
{
	GENERATED_BODY()

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

	
	//Derived Stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	TMap<EDamageType, int32> ResistanceMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxHealth; //base value should be 50 + (Endurance * 10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxMagic; //base value should be 50 + (Intelligence * 10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float DamageBoost; //is equal to .01 * Strength - each point grants a 1% boost to weapon damage

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	int ManaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	int ManaRegenSpeed;

	//Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AItem*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TMap<EEquipmentSlot, AItem*> EquippedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TMap<FString, int> AmmoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	int Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	AAbility* EquippedAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class AAbility>> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	TSubclassOf<AAbility> SelectedAbility;
};

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

	virtual void PostInitializeComponents() override;

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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

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

	UFUNCTION(BlueprintCallable)
	void CalculateDerivedStats();

	UFUNCTION(BlueprintCallable)
	bool TraceUnderCrosshairs(float TraceRange, FHitResult& OutHitResult, FVector& OutHitLocation);

	void TraceForItems();

	void TraceForInteractables();

	UFUNCTION(BlueprintCallable)
	void EquipItem(EEquipmentSlot EquipmentSlot, AItem* ItemToEquip);

	void PickupItem(AItem* ItemToPickup);

	void SpawnDefaultWeapon();

	UFUNCTION(BlueprintCallable)
	void SpawnSelectedAbility();

	void StartAttackTimer();
	void StartSecondaryTimer();

	void ResetCanAttack();
	void ResetSecondaryReady();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void DropItem(AItem* ItemToDrop);

	UFUNCTION(BlueprintCallable)
	void PayManaCost(float ManaCost);

	UFUNCTION(BlueprintCallable)
	void FillPlayerInfo();

	UFUNCTION(BlueprintCallable)
	void PullPlayerInfo();

	void GainMana();

	void StartManaRegenTimer(float RegenRate);

	void ResetHurtSound();

	void StartHurtTimer(float HurtTime);

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshPlayerSkills();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	class USceneComponent* MainHandSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	USceneComponent* ProjectileSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	class UActorAbilities* PlayerAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (AllowPrivateAccess = true))
	class USoundBase* PlayerHurtSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Saving", meta = (AllowPrivateAccess = true))
	FPlayerInfo PlayerInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float RotationRate = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = true))
	float InteractReach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Name", meta = (AllowPrivateAccess = true))
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxHealth; //base value should be 50 + (Endurance * 10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Magic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxMagic; //base value should be 50 + (Intelligence * 10)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float DamageBoost; //add Strength

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Regen", meta = (AllowPrivateAccess = true))
	int ManaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana Regen", meta = (AllowPrivateAccess = true))
	int ManaRegenSpeed;
	

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Scores", meta = (AllowPrivateAccess = true))
	int StatPoints;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TMap<FString, int> AmmoMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	AAbility* EquippedAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bCanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bSecondaryReady;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle AttackTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle SecondaryAttackTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle ManaRegenTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	FTimerHandle PlayerHurtTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = true))
	bool bHurtSoundCanPlay;

	class UDelveSaveGame* CharacterSave;
	
public:	
	FORCEINLINE int GetHealth() {return Health;}
	FORCEINLINE int GetMaxHealth() {return MaxHealth;}
	FORCEINLINE int GetMagic() {return Magic;}
	FORCEINLINE int GetMaxMagic() {return MaxMagic;}
	FORCEINLINE int GetGold() {return Gold;}
	FORCEINLINE bool GetDead() {return bDead;}
	FORCEINLINE bool GetInteractTrace() {return bInteractTrace;}
	FORCEINLINE bool GetItemTrace() {return bTraceForItems;}
	FORCEINLINE USceneComponent* GetProjectileSpawn() {return ProjectileSpawn;}
	FORCEINLINE USceneComponent* GetMainHandSpawn() {return MainHandSpawn;}
	FORCEINLINE TMap<EEquipmentSlot, AItem*> GetEquippedItems() {return EquippedItems;}

	void SetGold(int Value);
	void SetInteractTrace(bool Trace);
	void SetItemTrace(bool Trace);
	void SetDead(bool Death);

	virtual void Harmed_Implementation(FHitResult HitResult) override;
};
