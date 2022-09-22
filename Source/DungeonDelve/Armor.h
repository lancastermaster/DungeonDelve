// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DelveDamageType.h"
#include "EquipmentSlot.h"
#include "Armor.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FArmorInfo
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
	int Defence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
	TMap<EDamageType, int32> ResistanceMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
	EEquipmentSlot ArmorSlot;
};


UCLASS()
class DUNGEONDELVE_API AArmor : public AItem
{
	GENERATED_BODY()

	public:

	protected:

	private:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
		FArmorInfo ArmorInfo;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
		int Defence;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
		TMap<EDamageType, int32> ResistanceMap;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties", meta = (AllowPrivateAccess = true))
		EEquipmentSlot ArmorSlot;
	
};
