// Fill out your copyright notice in the Description page of Project Settings.


#include "LootContainer.h"
#include "InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Item.h"
#include "Armor.h"
#include "Weapon.h"
#include "PlayerCharacter.h"

// Sets default values
ALootContainer::ALootContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));
	RootComponent = DetectionSphere;

	ContainerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Container Mesh"));
	ContainerMesh -> SetupAttachment(RootComponent);

	LootParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Loot Particles"));
	LootParticles -> SetupAttachment(ContainerMesh);
}

// Called when the game starts or when spawned
void ALootContainer::BeginPlay()
{
	Super::BeginPlay();

	FillInventory();

	DetectionSphere->OnComponentBeginOverlap.AddDynamic(
		this, &ALootContainer::DetectionSphereOverlap
	);

	DetectionSphere->OnComponentEndOverlap.AddDynamic(
		this, &ALootContainer::DetectionSphereEndOverlap
	);
}

// Called every frame
void ALootContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALootContainer::FillInventory()
{
	int RandomIndex = PotentialLoot.Num()-1;
	FVector ContainerLocation = GetActorLocation();

	if(PotentialLoot.Num() == 0)return;
	if(!InventoryComp)return;

	for(int i = 0; i < LootNumber; i++)
	{
		AItem* SpawnedItem = GetWorld()->SpawnActor<AItem>(PotentialLoot[RandomIndex]);

		auto Armor = Cast<AArmor>(SpawnedItem);
		auto Weapon = Cast<AWeapon>(SpawnedItem);

		if(Armor)
		{
			Armor->SetItemState(EItemState::EIS_Carried);
			Armor->SetActorLocation(ContainerLocation);
			Armor->SetOwner(this);
			InventoryComp->AddToInventory(Armor);
		}
		else if(Weapon)
		{
			Weapon->SetItemState(EItemState::EIS_Carried);
			Weapon->SetActorLocation(ContainerLocation);
			Weapon->SetOwner(this);
			InventoryComp->AddToInventory(Weapon);
		}
		else
		{
			SpawnedItem->SetItemState(EItemState::EIS_Carried);
			SpawnedItem->SetActorLocation(ContainerLocation);
			SpawnedItem->SetOwner(this);
			InventoryComp->AddToInventory(SpawnedItem);
		}
	}
}

void ALootContainer::DetectionSphereOverlap(
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
			Player->SetInteractTrace(true);
		}
	}

void ALootContainer::DetectionSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
	{
		auto Player = Cast<APlayerCharacter>(OtherActor);
		if(Player)
		{
			Player->SetInteractTrace(false);
		}
	}