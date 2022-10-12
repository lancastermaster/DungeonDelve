// Fill out your copyright notice in the Description page of Project Settings.


#include "DelveGameInstance.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UDelveGameInstance::Init()
{
    Super::Init();

    PlayerRef = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}