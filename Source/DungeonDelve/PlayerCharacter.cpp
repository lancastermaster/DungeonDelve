// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Ability.h"
#include "ActorAbilities.h"
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
#include "DelvePlayerController.h"
#include "DelveGameInstance.h"
#include "DelveSaveGame.h"


// Sets default values
APlayerCharacter::APlayerCharacter():

StatPoints(0),
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

	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn"));
	ProjectileSpawn ->SetupAttachment(Camera);

	PlayerAbilities = CreateDefaultSubobject<UActorAbilities>(TEXT("Player Abilities"));
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadGame();
	
	StartManaRegenTimer(ManaRegenSpeed);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForInteractables();
	TraceForItems();
}

void APlayerCharacter::CalculateDerivedStats()
{
	MaxHealth = 50 + (Endurance * 10);
	MaxMagic = 50 + (Intelligence * 10);
	DamageBoost = FMath::RoundToInt(Strength * .01f);
	Defence = FMath::RoundToInt(Agility/5);
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

float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int InDamage = FMath::RoundToInt(DamageAmount);
	int DamageTaken = InDamage - Defence;

	if(bHurtSoundCanPlay)
	{
		UGameplayStatics::PlaySound2D(this, PlayerHurtSound);
		bHurtSoundCanPlay = false;
		StartHurtTimer(1.f);
	}

	if(DamageTaken <= 0)return DamageTaken;
	if(Health - DamageTaken <= 0)
	{
		Health = 0;
		bDead = true;
		Die();
	}
	else
	{
		Health -= DamageTaken;
	}
	return DamageTaken;
}

void APlayerCharacter::StartHurtTimer(float HurtTime)
{
	GetWorldTimerManager().SetTimer(PlayerHurtTimer, this, &APlayerCharacter::ResetHurtSound, HurtTime);
}

void APlayerCharacter::ResetHurtSound()
{
	bHurtSoundCanPlay = true;
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
	PayManaCost(EquippedAbility->GetAbilityInfo().ManaCost);
	EquippedAbility->BeginActivation();
}

void APlayerCharacter::LeftClickDown()
{
	bPrimaryDown = true;
	StartAttackTimer();
}

void APlayerCharacter::LeftClickUp()
{
	bPrimaryDown = false;
}

void APlayerCharacter::RightClickDown()
{
	bSecondaryDown = true;
	StartSecondaryTimer();
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
		TraceHitItem = nullptr;
	}

	if(TraceHitActor)
	{
		CallInteract(TraceHitActor);
	}

	if(TraceHitActor == nullptr && TraceHitItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's null, Jim."));
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

	if(EquippedItems.Contains(EquipmentSlot))
	{
		//Move OriginalItem to the Inventory from its slot
		//Move ItemToEquip from Inventory to the slot
		AItem* OriginalItem = EquippedItems[EquipmentSlot];
		
		OriginalItem->SetItemState(EItemState::EIS_Carried);
		EquippedItems[EquipmentSlot] = ItemToEquip;
		ItemToEquip->SetItemState(EItemState::EIS_Equipped);
		Inventory.Remove(ItemToEquip);
		Inventory.Add(OriginalItem);
	}
	else
	{
		EquippedItems.Add(EquipmentSlot, ItemToEquip);
		ItemToEquip->SetItemState(EItemState::EIS_Equipped);
		Inventory.Remove(ItemToEquip);
	}
}

void APlayerCharacter::PickupItem(AItem* ItemToPickup)
{
	if(ItemToPickup->GetPickupSound())
	{
		UGameplayStatics::PlaySound2D(this, ItemToPickup->GetPickupSound());
	}
	if(ItemToPickup->GetItemName() == TEXT("Gold"))
	{
		SetGold(ItemToPickup->GetItemValue());
		
		ItemToPickup->Destroy();
	}
	else if(ItemToPickup->GetItemType() == EItemType::EIT_Ammo)
	{
		if(AmmoMap.Contains(ItemToPickup->GetItemName()))
		{
			AmmoMap[ItemToPickup->GetItemName()] += ItemToPickup->GetItemQuantity();
		}
		else
		{
			AmmoMap.Add(ItemToPickup->GetItemName(), ItemToPickup->GetItemQuantity());
		}

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
			ItemToPickup->SetPlayerRef();

			SetInventoryItemReference(ItemToPickup);
		}
		return;
	}
}

void APlayerCharacter::SpawnDefaultWeapon()
{
	if(EquippedItems.Contains(EEquipmentSlot::EES_MainHand))return;
	if(!DefaultWeapon)return;
	AItem* SpawnedDefault = GetWorld()->SpawnActor<AItem>(DefaultWeapon);

	Inventory.Add(SpawnedDefault);
	SpawnedDefault->AttachToComponent(MainHandSpawn, FAttachmentTransformRules::KeepRelativeTransform);
	SpawnedDefault->SetOwner(this);
	SpawnedDefault->SetItemState(EItemState::EIS_Carried);
			
	EquipItem(EEquipmentSlot::EES_MainHand, SpawnedDefault);
}

void APlayerCharacter::SpawnSelectedAbility()
{
	if(!PlayerAbilities)return;
	if(!PlayerAbilities->GetSelectedAbility())return;

	AAbility* SpawnedAbility = GetWorld()->SpawnActorDeferred<AAbility>(
		PlayerAbilities->GetSelectedAbility(), 
		ProjectileSpawn->GetRelativeTransform(), 
		this,
		this,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	SpawnedAbility->AttachToComponent(ProjectileSpawn, FAttachmentTransformRules::KeepWorldTransform);
	SpawnedAbility->SetOwner(this);

	EquippedAbility = SpawnedAbility;
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
	if(!EquippedAbility)return;
	
	if(Magic - (EquippedAbility->GetAbilityInfo().ManaCost) >= 0)
	{
		SecondaryAction();
		bSecondaryReady = false;

		GetWorldTimerManager().SetTimer(
			SecondaryAttackTimer, 
			this, 
			&APlayerCharacter::ResetSecondaryReady, 
			EquippedAbility->GetAbilityInfo().CoolDown
			);
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

	auto PC = Cast<ADelvePlayerController>(this->GetController());

	if(PC)
	{
		PC->DisableInput(PC);
		PC->bShowMouseCursor = true;
	}
}

void APlayerCharacter::SetDead(bool Death)
{
	bDead = Death;
}

void APlayerCharacter::DropItem(AItem* ItemToDrop)
{
	if(Inventory.Contains(ItemToDrop))
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);

		ItemToDrop->DetachFromActor(DetachmentTransformRules);
		ItemToDrop->SetItemState(EItemState::EIS_Falling);
		//ItemToDrop->ThrowItem();
		Inventory.RemoveSingle(ItemToDrop);
		ItemToDrop = nullptr;
	}
}

void APlayerCharacter::PayManaCost(float ManaCost)
{
	if(Magic >= ManaCost)
	{
		Magic -= ManaCost;
	}
}

void APlayerCharacter::GainMana()
{
	if(Magic < MaxMagic)
	{
		if(Magic + ManaRegen >= MaxMagic)
		{
			Magic = MaxMagic;
		}
		else
		{
			Magic += ManaRegen;
		}
	}
}

void APlayerCharacter::StartManaRegenTimer(float RegenRate)
{
	GetWorldTimerManager().SetTimer(ManaRegenTimer, this, &APlayerCharacter::GainMana, RegenRate, true);
}

void APlayerCharacter::FillPlayerInfo()
{
	//Ability Scores
	PlayerInfo.Strength = Strength;
	PlayerInfo.Endurance = Endurance;
	PlayerInfo.Agility = Agility;
	PlayerInfo.Intelligence = Intelligence;
	
	//Derived Stats
	PlayerInfo.Health = Health;
	PlayerInfo.MaxHealth = MaxHealth;
	PlayerInfo.Magic = Magic;
	PlayerInfo.MaxMagic = Magic;
	PlayerInfo.DamageBoost = DamageBoost;

	for(auto Resist : ResistanceMap)
	{
		PlayerInfo.ResistanceMap.Add(Resist.Key, Resist.Value);
	}

	//Inventory
	PlayerInfo.Gold = Gold;

	//Combat
	PlayerInfo.Defence = Defence;
	PlayerInfo.SelectedAbility = PlayerAbilities->GetSelectedAbility();

	/*for(TSubclassOf<AAbility> AbilityClass : PlayerAbilities->GetLearnedAbilities())
	{
		PlayerInfo.Abilities.Add(AbilityClass);
	}*/

	PlayerInfo.ManaRegen = ManaRegen;
	PlayerInfo.ManaRegenSpeed = ManaRegenSpeed;
}

void APlayerCharacter::PullPlayerInfo()
{
	//Ability Scores
		Strength = PlayerInfo.Strength;
		Endurance = PlayerInfo.Endurance;
		Agility = PlayerInfo.Agility;
		Intelligence = PlayerInfo.Intelligence;

		//Derived Stats
		Health = PlayerInfo.Health;
		MaxHealth = PlayerInfo.MaxHealth;
		Magic = PlayerInfo.Magic;
		MaxMagic = PlayerInfo.MaxMagic;
		DamageBoost = PlayerInfo.DamageBoost;

		for(auto Resist : PlayerInfo.ResistanceMap)
		{
			ResistanceMap.Add(Resist.Key, Resist.Value);
		}

		//Inventory
		Gold = PlayerInfo.Gold;

		//Combat
		Defence = PlayerInfo.Defence;
		PlayerAbilities->SetSelectedAbility(PlayerInfo.SelectedAbility);
		ManaRegen = PlayerInfo.ManaRegen;
		ManaRegenSpeed = PlayerInfo.ManaRegenSpeed;

		/*for(TSubclassOf<AAbility> AbilityClass : PlayerInfo.Abilities)
		{
			PlayerAbilities -> AddAbility(AbilityClass);
		}*/
}

void APlayerCharacter::SaveGame()
{
	UDelveGameInstance* GameInstance = Cast<UDelveGameInstance>(GetGameInstance());

	UDelveSaveGame* SaveGameInstance = GameInstance->GetSaveGame();

	FillPlayerInfo();

	if(!SaveGameInstance)
	{
		SaveGameInstance = Cast<UDelveSaveGame>(UGameplayStatics::CreateSaveGameObject(UDelveSaveGame::StaticClass()));
	}

	SaveGameInstance->SetInfoToSave(PlayerInfo);

	for(AItem* Item : Inventory)
	{
		SaveGameInstance->GetInventoryClasses().Add(Item->GetClass());
	}

	if(EquippedItems.Contains(EEquipmentSlot::EES_Head))
	{
		SaveGameInstance->SetPlayerHead(EquippedItems[EEquipmentSlot::EES_Head]->GetClass());
	}
	if(EquippedItems.Contains(EEquipmentSlot::EES_Torso))
	{
		SaveGameInstance->SetPlayerTorso(EquippedItems[EEquipmentSlot::EES_Torso]->GetClass());
	}
	if(EquippedItems.Contains(EEquipmentSlot::EES_Hands))
	{
		SaveGameInstance->SetPlayerHands(EquippedItems[EEquipmentSlot::EES_Hands]->GetClass());
	}
	if(EquippedItems.Contains(EEquipmentSlot::EES_Feet))
	{
		SaveGameInstance->SetPlayerFeet(EquippedItems[EEquipmentSlot::EES_Feet]->GetClass());
	}
	if(EquippedItems.Contains(EEquipmentSlot::EES_MainHand))
	{
		SaveGameInstance->SetPlayerMainhand(EquippedItems[EEquipmentSlot::EES_MainHand]->GetClass());
	}
	if(EquippedItems.Contains(EEquipmentSlot::EES_Offhand))
	{
		SaveGameInstance->SetPlayerOffhand(EquippedItems[EEquipmentSlot::EES_Offhand]->GetClass());
	}
	if(EquippedItems.Contains(EEquipmentSlot::EES_Ring))
	{
		SaveGameInstance->SetPlayerRing(EquippedItems[EEquipmentSlot::EES_Ring]->GetClass());
	}
	if(EquippedItems.Contains(EEquipmentSlot::EES_Neck))
	{
		SaveGameInstance->SetPlayerNecklace(EquippedItems[EEquipmentSlot::EES_Neck]->GetClass());
	}

	for(auto AbilityClass : PlayerAbilities->GetLearnedAbilities())
	{
		SaveGameInstance->GetAbilityClasses().Add(AbilityClass);
	}

	UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, GameInstance->GetSaveSlot(), 0);
}

void APlayerCharacter::LoadGame()
{
	UDelveGameInstance* GameInstance = Cast<UDelveGameInstance>(GetGameInstance());

	CharacterSave = Cast<UDelveSaveGame>(UGameplayStatics::LoadGameFromSlot(GameInstance->GetSaveSlot(), 0));
	if(CharacterSave)
	{
		PlayerInfo = CharacterSave->GetPlayerInfoToLoad();

		PullPlayerInfo();

		if(CharacterSave->GetHeadInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_Head, GetWorld()->SpawnActor<AArmor>(CharacterSave->GetHeadInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		if(CharacterSave->GetTorsoInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_Torso, GetWorld()->SpawnActor<AArmor>(CharacterSave->GetTorsoInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		if(CharacterSave->GetHandsInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_Hands, GetWorld()->SpawnActor<AArmor>(CharacterSave->GetHandsInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		if(CharacterSave->GetFeetInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_Feet, GetWorld()->SpawnActor<AArmor>(CharacterSave->GetFeetInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		if(CharacterSave->GetMainhandInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_MainHand, GetWorld()->SpawnActor<AWeapon>(CharacterSave->GetMainhandInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		if(CharacterSave->GetOffhandInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_Offhand, GetWorld()->SpawnActor<AArmor>(CharacterSave->GetOffhandInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		if(CharacterSave->GetRingInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_Ring, GetWorld()->SpawnActor<AArmor>(CharacterSave->GetRingInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		if(CharacterSave->GetNeckInfo())
		{
			EquippedItems.Add(EEquipmentSlot::EES_Neck, GetWorld()->SpawnActor<AArmor>(CharacterSave->GetNeckInfo(), MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation()));
		}
		
		for(auto Entry : EquippedItems)
		{
			Entry.Value->AttachToComponent(MainHandSpawn, FAttachmentTransformRules::KeepRelativeTransform);
			Entry.Value->SetOwner(this);
			Entry.Value->SetItemState(EItemState::EIS_Equipped);
		}

		for(auto Entry : CharacterSave->GetInventoryClasses())
		{
			auto Item = GetWorld()->SpawnActor<AItem>(Entry, MainHandSpawn->GetComponentLocation(), MainHandSpawn->GetComponentRotation());
			Inventory.Add(Item);
			Item->AttachToComponent(MainHandSpawn, FAttachmentTransformRules::KeepRelativeTransform);
			Item->SetOwner(this);
			Item->SetItemState(EItemState::EIS_Carried);
			Item->SetPlayerRef();

			SetInventoryItemReference(Item);
		}

		for(auto Entry : CharacterSave->GetAbilityClasses())
		{
			PlayerAbilities->AddAbility(Entry);
		}

		UE_LOG(LogTemp, Warning, TEXT("Loading was successful."));
	}
	else
	{
		UDelveSaveGame* LoadedGame = Cast<UDelveSaveGame>(UGameplayStatics::CreateSaveGameObject(UDelveSaveGame::StaticClass()));
		CharacterSave = LoadedGame;

		UE_LOG(LogTemp, Warning, TEXT("Loading not successful."));

		SpawnDefaultWeapon();
		SpawnSelectedAbility();
		CalculateDerivedStats();
		Health = MaxHealth;
		Magic = MaxMagic;
	}
}