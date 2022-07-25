// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayerAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"
#include "Armor.h"
#include "Weapon.h"
#include "Components/SceneComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(RootComponent);

	MainHandSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Main Hand Spawn"));
	MainHandSpawn -> SetupAttachment(RootComponent);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	PlayerAttributes = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("PlayerAttributes"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeDefaultAttributesAbilities();

	InitializeDerivedStats();
	
	Health = MaxHealth;
	Magic = MaxMagic;
	CarryWeight = 0;
	Gold = 0;
}

void APlayerCharacter::InitializeDefaultAttributesAbilities()
{
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);

		//apply default attributes
		if(DefaultAttributeEffect)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

			if(SpecHandle.IsValid())
			{
				FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}

		//give abilities
		if(HasAuthority())
		{
			for(TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbilities)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility.GetDefaultObject(),1,0));
			}
		}
	}
}

UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForItems();
}

void APlayerCharacter::InitializeDerivedStats()
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	MaxHealth = 50 + Endurance * 10;
	MaxMagic = 50 + (Presence * 5) + (Intelligence * 5);
	MaxCarryWeight = 50 + (Strength * 5) + (Endurance * 5);
	MovementSpeed = Agility * 100;
	JumpHeight = Agility * 90;

	MoveComp->MaxWalkSpeed = MovementSpeed;
	MoveComp->MaxWalkSpeedCrouched = MovementSpeed / 2;
	MoveComp->JumpZVelocity = JumpHeight;
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
	PlayerInputComponent->BindAction(TEXT("PrimaryAction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::PrimaryAction);
	PlayerInputComponent->BindAction(TEXT("SecondaryAction"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SecondaryAction);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
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
	if(EquippedItems.Contains(EEquipmentSlot::EES_MainHand))
	{
		AWeapon* MainHand = Cast<AWeapon>(EquippedItems[EEquipmentSlot::EES_MainHand]);
		if(MainHand)
		{
			MainHand->Attack();
		}
	}
}

void APlayerCharacter::SecondaryAction()
{
	
}

void APlayerCharacter::Interact()
{
	if(TraceHitItem != nullptr)
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

void APlayerCharacter::SetGold(int Value)
{
	Gold += Value;
}

void APlayerCharacter::EquipItem(EEquipmentSlot EquipmentSlot, AItem* ItemToEquip)
{
	EquippedItems.Add(EquipmentSlot, ItemToEquip);
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
		Inventory.Add(ItemToPickup);
		ItemToPickup->AttachToComponent(MainHandSpawn, FAttachmentTransformRules::KeepRelativeTransform);
		ItemToPickup->SetOwner(this);
		ItemToPickup->SetItemState(EItemState::EIS_Carried);
		//TraceHitItem->Destroy();

		UE_LOG(LogTemp, Warning, TEXT("%s added to Inventory"), *ItemToPickup->GetItemName());
	}
}