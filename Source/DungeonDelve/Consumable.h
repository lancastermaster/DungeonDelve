// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Consumable.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONDELVE_API AConsumable : public AItem
{
	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Consume();

	protected:

	private:

	public:
	
};
