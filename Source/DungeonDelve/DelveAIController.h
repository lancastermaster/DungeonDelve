// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DelveAIController.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONDELVE_API ADelveAIController : public AAIController
{
	GENERATED_BODY()
	
	public:

		ADelveAIController();

		virtual void OnPossess(APawn* InPawn) override;

	private:
		UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = true))
		class UBlackboardComponent* BlackboardComponent;

		UPROPERTY(BlueprintReadWrite, Category = "AI Behavior", meta = (AllowPrivateAccess = true))
		class UBehaviorTreeComponent* BehaviorTreeComp;

	public:
		FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const {return BlackboardComponent;}
};
