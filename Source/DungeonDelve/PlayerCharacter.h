// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DelveDamageType.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include <GameplayEffectTypes.h>
#include "Item.h"
#include "EquipmentSlot.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class DUNGEONDELVE_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilties, meta = (AllowPrivateAccess = true))
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class UPlayerAttributeSet* PlayerAttributes;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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
	
	void Interact();

	void InitializeDerivedStats();

	void InitializeDefaultAttributesAbilities();

	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);

	void TraceForItems();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = true))
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	float RotationRate = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = true))
	float InteractReach;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxHealth; //base value should be 50 + Endurance * 10

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int Magic;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	int MaxMagic; //base value should be 50 + (Presence * 5) + (Intelligence * 5)

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float MaxCarryWeight; //add strength * 5 and endurance * 5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float CarryWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float MovementSpeed; //add Agility * 10

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float JumpHeight; //add Agility * 10

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Derived Stats", meta = (AllowPrivateAccess = true))
	float DamageBoost; //add Strength

	int Defence;

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

	TMap<EDamageType, int32> ResistanceMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items", meta = (AllowPrivateAccess = true))
	bool bTraceForItems;

	int8 OverlappedItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	class AItem* TraceHitItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items", meta = (AllowPrivateAccess = true))
	AItem* TraceHitItemLastFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TArray<AItem*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = true))
	TMap<EEquipmentSlot, AItem*> EquippedItems;
	
public:	
	FORCEINLINE int GetHealth() {return Health;}
	FORCEINLINE int GetMaxHealth() {return MaxHealth;}
	FORCEINLINE int GetMagic() {return Magic;}
	FORCEINLINE int GetMaxMagic() {return MaxMagic;}
	FORCEINLINE int GetGold() {return Gold;}

	//Effect that initializes our default attributes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	//Array of starting abilities
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	void SetGold(int Value);
};
