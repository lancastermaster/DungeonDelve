// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "PlayerCharacter.h"


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

	ItemLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Item Light"));
	ItemLight -> SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	SetItemProperties(EItemState::EIS_Pickup);
	PickupWidget->SetVisibility(false);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateTowardsPlayer();
}

void AItem::RotateTowardsPlayer()
{
	if(!ItemSprite)return;

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	FVector ItemLocation = GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ItemLocation,CameraManager->GetCameraLocation());

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = LookAtRotation.Yaw - 90;

	ItemSprite->SetWorldRotation(DeltaRotation);
}

void AItem::SetItemState(EItemState NewState)
{
	ItemState = NewState;
	SetItemProperties(ItemState);
}

void AItem::SetItemProperties(EItemState State)
{
	switch(State)
	{
		case EItemState::EIS_Pickup:
			ItemSprite->SetSimulatePhysics(false);
			ItemSprite->SetEnableGravity(false);
			ItemSprite->SetVisibility(true);
			ItemSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			PickupRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			PickupRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			ItemBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemBox->SetCollisionResponseToChannel(
				ECollisionChannel::ECC_Visibility,
				ECollisionResponse::ECR_Block);

			ItemBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

			ItemLight->SetVisibility(true);
		break;

		case EItemState::EIS_Carried:
			ItemSprite->SetSimulatePhysics(false);
			ItemSprite->SetEnableGravity(false);
			ItemSprite->SetVisibility(false);
			ItemSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			PickupRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			PickupRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			ItemBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			ItemLight->SetVisibility(false);
		break;

		case EItemState::EIS_Equipped:
			ItemSprite->SetSimulatePhysics(false);
			ItemSprite->SetEnableGravity(false);
			ItemSprite->SetVisibility(true);
			ItemSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			PickupRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			PickupRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			ItemBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			ItemLight->SetVisibility(false);
		break;

		case EItemState::EIS_Falling:
			ItemSprite->SetSimulatePhysics(true);
			ItemSprite->SetEnableGravity(true);
			ItemSprite->SetVisibility(true);
			ItemSprite->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			PickupRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			PickupRadius->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			ItemBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemBox->SetCollisionResponseToChannel(
				ECollisionChannel::ECC_Visibility,
				ECollisionResponse::ECR_Block);

			ItemBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

			ItemLight->SetVisibility(true);
		break;
	}
}

void AItem::SetPlayerRef()
{
	PlayerRef = Cast<APlayerCharacter>(this->GetOwner());
}