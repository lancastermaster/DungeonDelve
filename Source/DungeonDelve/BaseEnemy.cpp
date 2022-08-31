// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperFlipbookComponent.h"
#include "DelveAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Projectile.h"
#include "Ability.h"
#include "ActorAbilities.h"


ABaseEnemy::ABaseEnemy() :

bCanAttack(true),
bDying(false),
AttackRate(1.f),
bTargetInRange(false)

{
    CombatRange = CreateDefaultSubobject<USphereComponent>(TEXT("CombatRange"));
    CombatRange -> SetupAttachment(RootComponent);

    ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawn"));
    ProjectileSpawn -> SetupAttachment(GetSprite());

    EnemyAbilities = CreateDefaultSubobject<UActorAbilities>(TEXT("Enemy Abilities"));
}

void ABaseEnemy::BeginPlay()
{
    Super::BeginPlay();

    CombatRange->OnComponentBeginOverlap.AddDynamic(
        this,
        &ABaseEnemy::CombatRangeOverlap);
    
    CombatRange->OnComponentEndOverlap.AddDynamic(
        this,
        &ABaseEnemy::CombatRangeEndOverlap);

    FVector WorldPatrolPoint = UKismetMathLibrary::TransformLocation(
        GetActorTransform(),
        PatrolPoint);

    DrawDebugSphere(
        GetWorld(),
        WorldPatrolPoint,
        25.f,
        12,
        FColor::Red,
        true
    );

    EnemyController = Cast<ADelveAIController>(GetController());
    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if(EnemyController)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyController is not NULL"));
        
        EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("StartPosition"), this->GetActorLocation());
        EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanAttack"), GetCanAttack());
        EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), bTargetInRange);
        EnemyController->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Player);
        EnemyController->GetBlackboardComponent()->SetValueAsVector(TEXT("TargetPosition"), WorldPatrolPoint);
        EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("PlayerDead"), Player->GetDead());

        EnemyController->RunBehaviorTree(GetBehaviorTree());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyController is NULL"));
    }
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

void ABaseEnemy::SpawnProjectile(USceneComponent* SpawnComp)
{
    FVector SpawnLocation = SpawnComp->GetComponentLocation();
    FRotator SpawnRotation = SpawnComp->GetComponentRotation();

    if(EnemyProjectile)
    {
        //PlayAttackSound(SpawnLocation);

        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(EnemyProjectile, SpawnLocation, SpawnRotation);
        Projectile->SetOwner(this);
    }

}

float ABaseEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    int HurtAmount = UKismetMathLibrary::Round(DamageAmount);
    HurtAmount-=Defence;

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

void ABaseEnemy::CombatRangeOverlap(
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
            bTargetInRange = true;
            if(EnemyController)
            {
                EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), bTargetInRange);
            }
        }
    }

void ABaseEnemy::CombatRangeEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex)
    {
        auto Player = Cast<APlayerCharacter>(OtherActor);

        if(Player)
        {
            bTargetInRange = false;
            if(EnemyController)
            {
                EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("InAttackRange"), bTargetInRange);
            }
        }
    }