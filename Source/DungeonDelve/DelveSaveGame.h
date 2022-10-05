// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerCharacter.h"
#include "DelveSaveGame.generated.h"



UCLASS()
class DUNGEONDELVE_API UDelveSaveGame : public USaveGame
{
	GENERATED_BODY()

	private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	struct FPlayerInfo PlayerInfoToSave;

	public:
	FORCEINLINE FPlayerInfo GetPlayerInfoToLoad() {return PlayerInfoToSave;}

	void SetInfoToSave(FPlayerInfo InfoToSave) {PlayerInfoToSave = InfoToSave;}
};
