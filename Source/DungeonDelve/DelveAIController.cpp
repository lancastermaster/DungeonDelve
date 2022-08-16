// Fill out your copyright notice in the Description page of Project Settings.


#include "DelveAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseEnemy.h"

ADelveAIController::ADelveAIController()
{
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
    check(BlackboardComponent);

    BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Comp"));
    check(BehaviorTreeComp);
}

void ADelveAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if(InPawn == nullptr)return;

    ABaseEnemy* Enemy = Cast<ABaseEnemy>(InPawn);
    if(Enemy)
    {
        if(Enemy->GetBehaviorTree())
        {
            BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->BlackboardAsset));
        }
    }
}