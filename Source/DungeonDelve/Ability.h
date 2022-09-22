// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	FText AbilityDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	float ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	float CastTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	float CoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	class UTexture2D* ItemIcon;
};

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	FAbilityInfo AbilityInfo;

	bool bCancelAbility;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
