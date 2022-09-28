// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	float Magnitude;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	bool bCancelAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info", meta = (AllowPrivateAccess = true))
	bool bChargedAbility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags", meta = (AllowPrivateAccess = true))
	FGameplayTagContainer TagContainer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE FAbilityInfo GetAbilityInfo() {return AbilityInfo;}
};
