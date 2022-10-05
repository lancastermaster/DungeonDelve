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
	TArray<TSubclassOf<class AAbility>> Abilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = true))
	TSubclassOf<AAbility> SelectedAbility;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<TSubclassOf<AAbility>> GetLearnedAbilities() {return Abilities;}

	FORCEINLINE TSubclassOf<AAbility> GetSelectedAbility() {return SelectedAbility;}

	void SetSelectedAbility(TSubclassOf<AAbility> AbilityToSet) {SelectedAbility = AbilityToSet;}
	void SetAbilities(TArray<TSubclassOf<class AAbility>> AbilitiesToLearn) {Abilities = AbilitiesToLearn;}
	void AddAbility(TSubclassOf<AAbility> AbilityToAdd);
};
