// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DelveDamageType.h"
#include "Armor.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONDELVE_API AArmor : public AItem
{
	GENERATED_BODY()

	public:

	protected:

	private:
		int Defence;

		TMap<EDamageType, int32> ResistanceMap;
	
};
