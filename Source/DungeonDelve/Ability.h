// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

UCLASS()
class DUNGEONDELVE_API AAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability Defaults")
	void BeginActivation();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability Defaults")
	void ActivateAbility();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability Defaults")
	void CancelAbility();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Ability Defaults")
	void CompleteActivation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
