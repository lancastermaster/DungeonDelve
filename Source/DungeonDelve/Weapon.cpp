// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "GameFramework/Controller.h"
#include "Projectile.h"
#include "HarmableInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseEnemy.h"
#include "GameFramework/DamageType.h"

AController* AWeapon::GetOwnerController() const
{
    APawn *OwnerPawn = Cast<APawn>(GetOwner());

    if(OwnerPawn == nullptr)return nullptr;
    
    return OwnerPawn -> GetController();
}

void AWeapon::PlayAttackSound(FVector SoundLocation)
{
    if(!AttackSound)return;
    UGameplayStatics::SpawnSound2D(GetWorld(), AttackSound);
}

void AWeapon::SpawnAttackParticles(UParticleSystem* Particles, FVector SpawnLocation, FRotator SpawnRotation)
{
    if(!AttackParticles)return;
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particles, SpawnLocation, SpawnRotation);
}

bool AWeapon::TraceUnderCrosshairs(float TraceRange, FHitResult& OutHitResult, FVector& OutHitLocation)
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

void AWeapon::ShootRaycast(USceneComponent* TraceStart, TSubclassOf<class UDamageType> DamageType)
{
    FVector SpawnLocation = TraceStart->GetComponentLocation();
    FHitResult RaycastHit;
    FVector HitLocation;

    PlayAttackSound(SpawnLocation);

    bool bBeamEnd = TraceUnderCrosshairs(WeaponRange, RaycastHit, HitLocation);
    if(bBeamEnd)
    {
        //does the hit actor implement the Harmable Interface?
        if(RaycastHit.Actor.IsValid())
        {
            IHarmableInterface* HarmableInterface = Cast<IHarmableInterface>(RaycastHit.Actor.Get());
            if(HarmableInterface)
            {
               auto HitEnemy = Cast<ABaseEnemy>(RaycastHit.Actor.Get());
               if(HitEnemy)
               {
                    UGameplayStatics::ApplyDamage(
                        HitEnemy,
                        UKismetMathLibrary::Round(GetDamage()),
                        this->GetOwnerController(),
                        this,
                        DamageType
                    );
               } 
            }
        }
        SpawnAttackParticles(AttackParticles, HitLocation, HitLocation.Rotation());
    }
}

void AWeapon::SpawnProjectile(USceneComponent* ProjectileSpawn)
{
    FVector SpawnLocation = ProjectileSpawn->GetComponentLocation();
    FRotator SpawnRotation = ProjectileSpawn->GetComponentRotation();

    if(WeaponProjectile)
    {
        PlayAttackSound(SpawnLocation);

        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(WeaponProjectile, SpawnLocation, SpawnRotation);
        Projectile->SetOwner(this->GetOwner());
    }
}