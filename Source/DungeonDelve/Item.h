// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
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

USTRUCT (BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int ItemValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	EItemType ItemType;
};

UCLASS()
class DUNGEONDELVE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPlayerRef();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RotateTowardsPlayer();

	void SetItemProperties(EItemState State);


	UFUNCTION()
	void PickupRadiusOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void PickupRadiusEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ThrowItem();

	void StopFalling();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UPointLightComponent* ItemLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX", meta = (AllowPrivateAccess = true))
	class USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FItemInfo ItemInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	float ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int ItemValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	int ItemQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	FString ItemDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	EItemState ItemState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	class APlayerCharacter* PlayerRef;

	bool bFalling;
	FTimerHandle ThrowItemTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags", meta = (AllowPrivateAccess = true))
	class FGameplayTagContainer TagContainer;

public:
	FORCEINLINE float GetItemWeight() const {return ItemWeight;}
	FORCEINLINE int GetItemValue() const {return ItemValue;}
	FORCEINLINE EItemState GetItemState() const {return ItemState;}
	FORCEINLINE EItemType GetItemType() const {return ItemType;}
	FORCEINLINE FString GetItemName() const {return ItemName;}
	FORCEINLINE APlayerCharacter* GetPlayerRef() const {return PlayerRef;}
	FORCEINLINE USoundBase* GetPickupSound() const {return PickupSound;}
	FORCEINLINE UPaperSpriteComponent* GetItemSprite() const {return ItemSprite;}
	FORCEINLINE int GetItemQuantity() const {return ItemQuantity;}

	void SetItemState(EItemState NewState);

};
