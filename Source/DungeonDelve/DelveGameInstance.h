// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DelveGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONDELVE_API UDelveGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:
	

	protected:
	virtual void Init() override;

	private:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game", meta = (AllowPrivateAccess = true))
		class UDelveSaveGame* SaveGame;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game", meta = (AllowPrivateAccess = true))
		FString SaveSlotName;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Ref", meta = (AllowPrivateAccess = true))
		class APlayerCharacter* PlayerRef;

	public:

		FORCEINLINE UDelveSaveGame* GetSaveGame() {return SaveGame;}
		FORCEINLINE FString GetSaveSlot() {return SaveSlotName;}
};
