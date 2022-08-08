// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DelveDamageType.h"
#include "StatusEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONDELVE_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusEffectComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitializeStatusEffects();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status Effects", meta = (AllowPrivateAccess = true))
	TMap<EDamageType, bool> StatusEffects;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TriggerStatusEffect(EDamageType DamageType, ACharacter* Owner, float StatusDamage);
};
