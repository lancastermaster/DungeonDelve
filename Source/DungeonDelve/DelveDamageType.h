#pragma once

UENUM(BlueprintType)
	enum class EDamageType : uint8 {
		EDT_Slashing = 0 UMETA(DisplayName = "Slashing"),
		EDT_Bludgeoning = 1 UMETA(DisplayName = "Bludgeoning"),
		EDT_Piercing = 2 UMETA(DisplayName = "Piercing"),
		EDT_Fire = 3 UMETA(DisplayName = "Fire"),
		EDT_Cold = 4 UMETA(DisplayName = "Cold"),
		EDT_Lightning = 5 UMETA(DisplayName = "Lightning"),
		EDT_Magic = 6 UMETA(DisplayName = "Magic"),
		EDT_MAX = 7 UMETA(DisplayName = "DefaultMax")
	};