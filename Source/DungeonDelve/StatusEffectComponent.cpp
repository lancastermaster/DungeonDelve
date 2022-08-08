// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffectComponent.h"
#include "DelveDamageType.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UStatusEffectComponent::UStatusEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	InitializeStatusEffects();

}


// Called every frame
void UStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatusEffectComponent::InitializeStatusEffects()
{
	StatusEffects.Add(EDamageType::EDT_Bludgeoning, false);
    StatusEffects.Add(EDamageType::EDT_Piercing, false);
    StatusEffects.Add(EDamageType::EDT_Slashing, false);
    StatusEffects.Add(EDamageType::EDT_Fire, false);
    StatusEffects.Add(EDamageType::EDT_Cold, false);
    StatusEffects.Add(EDamageType::EDT_Lightning, false);
    StatusEffects.Add(EDamageType::EDT_Magic, false);
}

void UStatusEffectComponent::TriggerStatusEffect(EDamageType DamageType, ACharacter* Owner, float StatusDamage)
{
	FTransform OwnerTransform = Owner->GetActorTransform();

	switch(DamageType)
	{
		case EDamageType::EDT_Bludgeoning:
		break;

		case EDamageType::EDT_Piercing:
		break;

		case EDamageType::EDT_Slashing:
		break;

		case EDamageType::EDT_Fire:
		break;

		case EDamageType::EDT_Cold:
		break;

		case EDamageType::EDT_Lightning:
		break;

		case EDamageType::EDT_Magic:
		break;
	}
}