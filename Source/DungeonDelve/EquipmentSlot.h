#pragma once

UENUM(BlueprintType)
	enum class EEquipmentSlot : uint8 {
		EES_Head = 0 UMETA(DisplayName = "Head"),
		EES_Torso = 1 UMETA(DisplayName = "Torso"),
		EES_Hands = 2 UMETA(DisplayName = "Hands"),
		EES_Feet = 3 UMETA(DisplayName = "Feet"),
		EES_MainHand = 4 UMETA(DisplayName = "MainHand"),
		EES_Offhand = 5 UMETA(DisplayName = "OffHand"),
		EES_Ring = 6 UMETA(DisplayName = "LeftRing"),
		EES_Neck = 7 UMETA(DisplayName = "Neck")
	};