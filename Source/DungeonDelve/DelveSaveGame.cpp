// Fill out your copyright notice in the Description page of Project Settings.


#include "DelveSaveGame.h"
#include "Ability.h"
#include "Armor.h"
#include "Consumable.h"
#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Weapon.h"

void UDelveSaveGame::AddAbilityToSave(TSubclassOf<AAbility> AddedAbility)
{
    AbilityClasses.Add(AddedAbility);
}

void UDelveSaveGame::AddItemToInventoryClasses(TSubclassOf<AItem> AddedItem)
{
    InventoryClasses.Add(AddedItem);
}