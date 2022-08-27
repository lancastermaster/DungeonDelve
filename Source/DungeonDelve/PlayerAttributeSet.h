// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DUNGEONDELVE_API UPlayerAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	public:

		UPlayerAttributeSet();

		UPROPERTY(BlueprintReadOnly, Category = "Derived Attributes")
		FGameplayAttributeData Health;
		ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Health);

		UPROPERTY(BlueprintReadOnly, Category = "Derived Attributes")
		FGameplayAttributeData MaxHealth;
		ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxHealth);

		UPROPERTY(BlueprintReadOnly, Category = "Derived Attributes")
		FGameplayAttributeData Magic;
		ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Magic);

		UPROPERTY(BlueprintReadOnly, Category = "Derived Attributes")
		FGameplayAttributeData MaxMagic;
		ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, MaxMagic);

		UPROPERTY(BlueprintReadOnly, Category = "Derived Attributes")
		FGameplayAttributeData DamageBoost;
		ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, DamageBoost);

		UPROPERTY(BlueprintReadOnly, Category = "Derived Attributes")
		FGameplayAttributeData Defence;
		ATTRIBUTE_ACCESSORS(UPlayerAttributeSet, Defence);
	protected:

	private:
};
