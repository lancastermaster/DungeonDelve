// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Armor.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Containers/Array.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HarmableInterface.h"
#include "InteractInterface.h"
#include "Item.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Ability.h"


// Sets default values
APlayerCharacter::APlayerCharacter():

Gold(0),
bCanAttack(true),
bSecondaryReady(true),
bDead(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(RootComponent);

	MainHandSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Main Hand Spawn"));
	MainHandSpawn -> SetupAttachment(Camera);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeDerivedStats();
	
	Health = MaxHealth;
	Magic = MaxMagic;

	SpawnDefaultWeapon();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForInteractables();
	//TraceForItems();

	if(bPrimaryDown)
	{
		StartAttackTimer();
	}

	if(bSecondaryDown)
	{
		StartSecondaryTimer();
	}
}

void APlayerCharacter::InitializeDerivedStats()
{
	MaxHealth = 50 + (Endurance * 10);
	MaxMagic = 50 + (Intelligence * 10);
	DamageBoost = Strength;
	ResistanceMap.Add(EDamageType::EDT_Bludgeoning, Agility);
	ResistanceMap.Add(EDamageType::EDT_Piercing, Agility);
	ResistanceMap.Add(EDamageType::EDT_Slashing, Agility);
	ResistanceMap.Add(EDamageType::EDT_Fire, Presence);
	ResistanceMap.Add(EDamageType::EDT_Cold, Presence);
	ResistanceMap.Add(EDamageType::EDT_Lightning, Presence);
	ResistanceMap.Add(EDamageType::EDT_Magic, Presence);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind Axis
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacter::LookRight);

	//Bind Actions
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("PrimaryAction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LeftClickDown);
	PlayerInputComponent->BindAction(TEXT("PrimaryAction"), EInputEvent::IE_Released, this, &APlayerCharacter::LeftClickUp);
	PlayerInputComponent->BindAction(TEXT("SecondaryAction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::RightClickDown);
	PlayerInputComponent->BindAction(TEXT("SecondaryAction"), EInputEvent::IE_Released, this, &APlayerCharacter::RightClickUp);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("ToggleInventory"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ToggleInventory);
	PlayerInputComponent->BindAction(TEXT("ToggleCharacterSheet"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ToggleCharacterSheet);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::PrimaryAction()
{
	
}

void APlayerCharacter::SecondaryAction()
{
	
}

void APlayerCharacter::LeftClickDown()
{
	bPrimaryDown = true;
}

void APlayerCharacter::LeftClickUp()
{
	bPrimaryDown = false;
}

void APlayerCharacter::RightClickDown()
{
	bSecondaryDown = true;
}

void APlayerCharacter::RightClickUp()
{
	bSecondaryDown = false;
}

void APlayerCharacter::Interact()
{
	if(TraceHitItem)
	{
		PickupItem(TraceHitItem);
	}
}

bool APlayerCharacter::TraceUnderCrosshairs(float TraceRange, FHitResult& OutHitResult, FVector& OutHitLocation)
{
	//get viewportsize
	FVector2D ViewPortSize;
    if(GEngine && GEngine->GameViewport)
    {
        GEngine->GameViewport->GetViewportSize(ViewPortSize);
    }

	//Get screen space location of crosshairs
    FVector2D CrossHairLocation(ViewPortSize.X / 2.f, (ViewPortSize.Y / 2.f));
    FVector CrosshairWorldPosition;
    FVector CrosshairWorldDirection;

	//Get world position & direction of crosshairs
    bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
        UGameplayStatics::GetPlayerController(this, 0), 
        CrossHairLocation, 
        CrosshairWorldPosition,
        CrosshairWorldDirection);

	if(bScreenToWorld)
	{
		//trace from crosshair world location outward
        FVector Start{CrosshairWorldPosition};
        FVector End {Start + CrosshairWorldDirection * TraceRange};
		OutHitLocation = End;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if(OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

void APlayerCharacter::TraceForItems()
{
	if(!bTraceForItems)return;

	FHitResult ItemTraceResult;
	FVector ItemTraceLocation;
	TraceUnderCrosshairs(InteractReach, ItemTraceResult, ItemTraceLocation);
	if(ItemTraceResult.bBlockingHit)
	{
		TraceHitItem = Cast<AItem>(ItemTraceResult.Actor);

		//store reference to HitItem for next frame
		TraceHitItemLastFrame = TraceHitItem;
	}
}

void APlayerCharacter::TraceForInteractables()
{
	if(!bInteractTrace)return;

	FHitResult InteractTraceResult;
	FVector InteractTraceLocation;
	TraceUnderCrosshairs(InteractReach, InteractTraceResult, InteractTraceLocation);
	if(InteractTraceResult.bBlockingHit)
	{
		if(InteractTraceResult.Actor.IsValid())
		{
			TraceHitActor = InteractTraceResult.Actor.Get();
			TraceHitItem = Cast<AItem>(TraceHitActor);
		}
	}
}

void APlayerCharacter::SetGold(int Value)
{
	Gold += Value;
}

void APlayerCharacter::SetInteractTrace(bool Trace)
{
	bInteractTrace = Trace;
}

void APlayerCharacter::SetItemTrace(bool Trace)
{
	bTraceForItems = Trace;
}

void APlayerCharacter::EquipItem(EEquipmentSlot EquipmentSlot, AItem* ItemToEquip)
{
	//EquippedItems[EquipmentSlot]->SetItemState(EItemState::EIS_Carried);
	EquippedItems.Add(EquipmentSlot, ItemToEquip);
	EquippedItems[EquipmentSlot]->SetItemState(EItemState::EIS_Equipped);
}

void APlayerCharacter::PickupItem(AItem* ItemToPickup)
{
	if(ItemToPickup->GetItemName() == TEXT("Gold"))
	{
		SetGold(ItemToPickup->GetItemValue());
		UE_LOG(LogTemp, Warning, TEXT("%d Gold in Inventory"), Gold);
		ItemToPickup->Destroy();
	}
	else
	{
		if(Inventory.Num() < 33)
		{
			Inventory.Add(ItemToPickup);
			ItemToPickup->AttachToComponent(MainHandSpawn, FAttachmentTransformRules::KeepRelativeTransform);
			ItemToPickup->SetOwner(this);
			ItemToPickup->SetItemState(EItemState::EIS_Carried);

			SetInventoryItemReference(ItemToPickup);
		}
		return;
	}
}

void APlayerCharacter::SpawnDefaultWeapon()
{
	if(!DefaultWeapon)return;
	AItem* SpawnedDefault = GetWorld()->SpawnActor<AItem>(DefaultWeapon);

	Inventory.Add(SpawnedDefault);
	SpawnedDefault->AttachToComponent(MainHandSpawn, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedDefault->SetOwner(this);
	SpawnedDefault->SetItemState(EItemState::EIS_Carried);
			
	EquipItem(EEquipmentSlot::EES_MainHand, SpawnedDefault);
	//SpawnedDefault->SetItemState(EItemState::EIS_Equipped);
}

void APlayerCharacter::StartAttackTimer()
{
	if(!bCanAttack)return;
	if(EquippedItems.Contains(EEquipmentSlot::EES_MainHand))
	{
		AWeapon* MainHand = Cast<AWeapon>(EquippedItems[EEquipmentSlot::EES_MainHand]);
		if(MainHand)
		{
			MainHand->Attack();
			bCanAttack = false;
			GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::ResetCanAttack, MainHand->GetAttackRate());
		}
	}
	else
	{
		
	}
}

void APlayerCharacter::StartSecondaryTimer()
{
	if(!bSecondaryReady)return;
	if(EquippedItems.Contains(EEquipmentSlot::EES_MainHand))
	{
		AWeapon* MainHand = Cast<AWeapon>(EquippedItems[EEquipmentSlot::EES_MainHand]);
		if(MainHand)
		{
			MainHand->SecondaryAbility();
			bSecondaryReady = false;
			GetWorldTimerManager().SetTimer(SecondaryAttackTimer, this, &APlayerCharacter::ResetSecondaryReady, MainHand->GetAttackRate());
		}
	}
	else
	{
		
	}
}

void APlayerCharacter::Harmed_Implementation(FHitResult HitResult)
{

}

void APlayerCharacter::ResetCanAttack()
{
	bCanAttack = true;
}

void APlayerCharacter::ResetSecondaryReady()
{
	bSecondaryReady = true;
}

void APlayerCharacter::Die()
{
	bDead = true;
}