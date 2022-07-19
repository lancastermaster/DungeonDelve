#pragma once

UENUM(BlueprintType)
	enum class EDamageType : uint8 {
		EDT_Slashing = 0 UMETA(DisplayName = "Slashing"),
		EDT_Bludgeoning = 1 UMETA(DisplayName = "Bludgeoning"),
		EDT_Piercing = 2 UMETA(DisplayName = "Piercing"),
		EDT_Burning = 3 UMETA(DisplayName = "Burning"),
		EDT_Freezing = 4 UMETA(DisplayName = "Freezing"),
		EDT_Shocking = 5 UMETA(DisplayName = "Shocking"),
		EDT_Rotting = 6 UMETA(DisplayName = "Rotting"),
		EDT_MAX = 7 UMETA(DisplayName = "DefaultMax")
	};