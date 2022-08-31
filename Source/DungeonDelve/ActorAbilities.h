// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorAbilities.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONDELVE_API UActorAbilities : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorAbilities();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	TMap<TSubclassOf<class AAbility>, bool> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	TSubclassOf<AAbility> SelectedAbility;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE TSubclassOf<AAbility> GetSelectedAbility() {return SelectedAbility;}

		
};
