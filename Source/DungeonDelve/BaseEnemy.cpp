// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperFlipbookComponent.h"

ABaseEnemy::ABaseEnemy()
{

}

void ABaseEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    RotateSpriteTorwardsPlayer();
}

void ABaseEnemy::RotateSpriteTorwardsPlayer()
{
    UPaperFlipbookComponent* EnemyFlipbook = GetSprite();
    if(EnemyFlipbook)
    {
        APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
        
	    FVector EnemyLocation = GetActorLocation();

	    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(EnemyLocation, CameraManager->GetCameraLocation());

	    FRotator DeltaRotation = FRotator::ZeroRotator;
	    DeltaRotation.Yaw = LookAtRotation.Yaw - 90;

	    EnemyFlipbook->SetWorldRotation(DeltaRotation);
    }
}

void ABaseEnemy::Harmed_Implementation(FHitResult HitResult)
{
    
}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    int HurtAmount = UKismetMathLibrary::Round(DamageAmount);

    if(EnemyController)
    {

    }
    if(Health - HurtAmount <= 0)
    {
        Health = 0;
        Die();
    }
    else
    {
        Health -= HurtAmount;
    }
    
    return DamageAmount;
}

void ABaseEnemy::Die()
{
    Destroy();
}