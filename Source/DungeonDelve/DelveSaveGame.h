// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerCharacter.h"
#include "EquipmentSlot.h"
#include "DelveSaveGame.generated.h"

USTRUCT (BlueprintType)
struct FInventoryInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AItem> ItemClass;
	
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

	//-----------------------Info for Armor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
	int Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
	TMap<EDamageType, int32> ResistanceMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
	EEquipmentSlot ArmorSlot;

	//-----------------------Info for Weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	bool bTwoHands = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	class UParticleSystem* AttackParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AProjectile> WeaponProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	TSubclassOf<class AAbility> WeaponAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDamageType> DamageType;
};

UCLASS()
class DUNGEONDELVE_API UDelveSaveGame : public USaveGame
{
	GENERATED_BODY()

	public:

	protected:
	
	private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	struct FPlayerInfo PlayerInfoToSave;

	//-----------No longer in use - might return to if needed
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//TMap<EEquipmentSlot, TSubclassOf<AItem>> PlayerEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AArmor> PlayerHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmor> PlayerTorso;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmor> PlayerHands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmor> PlayerFeet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AWeapon> PlayerMainhand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmor> PlayerOffhand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmor> PlayerRing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmor> PlayerNecklace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class AItem>> InventoryClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<AAbility>> AbilityClasses;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Ref", meta = (AllowPrivateAccess = true))
	//class APlayerCharacter* PlayerRef;

	public:
	FORCEINLINE FPlayerInfo GetPlayerInfoToLoad() {return PlayerInfoToSave;}
	FORCEINLINE TArray<TSubclassOf<AItem>> GetInventoryClasses() {return InventoryClasses;}
	FORCEINLINE TArray<TSubclassOf<AAbility>> GetAbilityClasses() {return AbilityClasses;}
	FORCEINLINE TSubclassOf<AArmor> GetHeadInfo() {return PlayerHead;}
	FORCEINLINE TSubclassOf<AArmor> GetTorsoInfo() {return PlayerTorso;}
	FORCEINLINE TSubclassOf<AArmor> GetHandsInfo() {return PlayerHands;}
	FORCEINLINE TSubclassOf<AArmor> GetFeetInfo() {return PlayerFeet;}
	FORCEINLINE TSubclassOf<AWeapon> GetMainhandInfo() {return PlayerMainhand;}
	FORCEINLINE TSubclassOf<AArmor> GetOffhandInfo() {return PlayerOffhand;}
	FORCEINLINE TSubclassOf<AArmor> GetRingInfo() {return PlayerRing;}
	FORCEINLINE TSubclassOf<AArmor> GetNeckInfo() {return PlayerNecklace;}

	void SetInfoToSave(FPlayerInfo InfoToSave) {PlayerInfoToSave = InfoToSave;}
	void SetPlayerHead(TSubclassOf<AArmor> ArmorClass) {PlayerHead = ArmorClass;}
	void SetPlayerTorso(TSubclassOf<AArmor> ArmorClass) {PlayerTorso = ArmorClass;}
	void SetPlayerHands(TSubclassOf<AArmor> ArmorClass) {PlayerHands = ArmorClass;}
	void SetPlayerFeet(TSubclassOf<AArmor> ArmorClass) {PlayerFeet = ArmorClass;}
	void SetPlayerMainhand(TSubclassOf<AWeapon> WeaponClass) {PlayerMainhand = WeaponClass;}
	void SetPlayerOffhand(TSubclassOf<AArmor> ArmorClass) {PlayerOffhand = ArmorClass;}
	void SetPlayerRing(TSubclassOf<AArmor> ArmorClass) {PlayerRing = ArmorClass;}
	void SetPlayerNecklace(TSubclassOf<AArmor> ArmorClass) {PlayerNecklace = ArmorClass;}
	void AddAbilityToSave(TSubclassOf<AAbility> AddedAbility);
	void AddItemToInventoryClasses(TSubclassOf<AItem> AddedItem);
};
