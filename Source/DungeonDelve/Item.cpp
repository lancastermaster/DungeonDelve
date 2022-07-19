// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	RootComponent = RootComp;

	ItemSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Item Sprite"));
	ItemSprite -> SetupAttachment(RootComp);

	ItemBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Item Box"));
	ItemBox -> SetupAttachment(RootComp);

	PickupRadius = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Radius"));
	PickupRadius -> SetupAttachment(RootComp);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickup Widget"));
	PickupWidget -> SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

