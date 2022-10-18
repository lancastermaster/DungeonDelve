// Fill out your copyright notice in the Description page of Project Settings.


#include "Vendor.h"
#include "InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Armor.h"
#include "Weapon.h"

// Sets default values
AVendor::AVendor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Componet"));
	RootComponent = CapsuleComp;

	VendorSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Vendor Sprite"));
	VendorSprite ->SetupAttachment(RootComponent);

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	DetectionSphere -> SetupAttachment(RootComponent);

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
}

// Called when the game starts or when spawned
void AVendor::BeginPlay()
{
	Super::BeginPlay();
	
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(
		this, &AVendor::DetectionSphereOverlap
	);

	DetectionSphere->OnComponentEndOverlap.AddDynamic(
		this, &AVendor::DetectionSphereEndOverlap
	);

	FillInventory();
}

// Called every frame
void AVendor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateSpriteTowardsPlayer();
}

void AVendor::RotateSpriteTowardsPlayer()
{
	if(!VendorSprite) return;

	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	FVector VendorLocation = GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(VendorLocation,CameraManager->GetCameraLocation());

	FRotator DeltaRotation = FRotator::ZeroRotator;
	DeltaRotation.Yaw = LookAtRotation.Yaw - 90;

	VendorSprite->SetWorldRotation(DeltaRotation);
}

void AVendor::FillInventory()
{
	if(!InventoryComp)return;
	
	for(int i = 0; i < 5; i++)
	{
		int RandNum = FMath::RandRange(0, PotentialVendorStock.Num() - 1);

		AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(PotentialVendorStock[RandNum]);

		auto Armor = Cast<AArmor>(SpawnedItem);
		auto Weapon = Cast<AWeapon>(SpawnedItem);

		if(Armor)
		{
			Armor->SetItemState(EItemState::EIS_Carried);

			FVector VendorLocation = GetActorLocation();
			Armor->SetActorLocation(VendorLocation);
			Armor->SetOwner(this);
			InventoryComp->AddToInventory(Armor);
		}
		else if(Weapon)
		{
			Weapon->SetItemState(EItemState::EIS_Carried);

			FVector VendorLocation = GetActorLocation();
			Weapon->SetActorLocation(VendorLocation);
			Weapon->SetOwner(this);
			InventoryComp->AddToInventory(Weapon);
		}
		else
		{
			SpawnedItem->SetItemState(EItemState::EIS_Carried);

			FVector VendorLocation = GetActorLocation();
			SpawnedItem->SetActorLocation(VendorLocation);
			SpawnedItem->SetOwner(this);
			InventoryComp->AddToInventory(SpawnedItem);
		}

	}



}

void AVendor::DetectionSphereOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bSweep, 
		const FHitResult& SweepResult)
	{
		auto Player = Cast<APlayerCharacter>(OtherActor);
		if(Player)
		{
			//Player->SetItemTrace(true);
			Player->SetInteractTrace(true);
			//PickupWidget->SetVisibility(true);
		}
	}

void AVendor::DetectionSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
	{
		auto Player = Cast<APlayerCharacter>(OtherActor);
		if(Player)
		{
			//Player->SetItemTrace(true);
			Player->SetInteractTrace(false);
			//PickupWidget->SetVisibility(true);
		}
	}