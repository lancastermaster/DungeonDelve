// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Controller.h"

AController* AWeapon::GetOwnerController() const
{
    APawn *OwnerPawn = Cast<APawn>(GetOwner());

    if(OwnerPawn == nullptr)return nullptr;
    
    return OwnerPawn -> GetController();
}