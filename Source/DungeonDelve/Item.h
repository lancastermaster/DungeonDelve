// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UENUM(BlueprintType)
	enum class EItemType : uint8 {
		EIT_Weapon = 0 UMETA(DisplayName = "Weapon"),
		EIT_Armor = 1 UMETA(DisplayName = "Armor"),
		EIT_Ammo = 2 UMETA(DisplayName = "Ammo"),
		EIT_Consumable = 3 UMETA(DisplayName = "Consumable"),

		EIT_MAX = 7 UMETA(DisplayName = "DefaultMax")
	};

UENUM(BlueprintType)
	enum class EItemState : uint8 {
		EIS_Pickup = 0 UMETA(DisplayName = "Pickup"),
		EIS_Carried = 1 UMETA(DisplayName = "Carried"),
		EIS_Equipped = 2 UMETA(DisplayName = "Equipped"),
		EIS_Falling = 3 UMETA(DisplayName = "Falling")
	};

/*
USTRUCT (BlueprintType)
struct FItemRarityTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GlowColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor DarkColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfStars;

};

//add in private section
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data Table", meta = (AllowPrivateAccess = true))
class UDataTable* ItemRarityDataTable;
*/

UCLASS()
class DUNGEONDELVE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateTowardsPlayer();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UWidgetComponent* PickupWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UBoxComponent* ItemBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class USphereComponent* PickupRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UPaperSpriteComponent* ItemSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (AllowPrivateAccess = true))
	class USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	float ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int ItemValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	EItemState ItemState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	EItemType ItemType;

public:
	FORCEINLINE float GetItemWeight() const {return ItemWeight;}
	FORCEINLINE int GetItemValue() const {return ItemValue;}
	FORCEINLINE EItemState GetItemState() const {return ItemState;}
	FORCEINLINE EItemType GetItemType() const {return ItemType;}
	FORCEINLINE FString GetItemName() const {return ItemName;}

};
