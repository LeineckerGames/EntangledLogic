#pragma once

// Keep UnlockAll last, its used to know the size of the enum
// when unlocking all through dev UI
UENUM()
enum class EUnlockables : uint8
{
	Unlocked,
	Factory_Y_Gate,
	Factory_Z_Gate,
	Factory_H_Gate,
	Factory_CNOT_Gate,
	Factory_QuantumGnome,
	Dev_Testing_Factories,
	Test_RainbowBaseGridColor,
	UnlockAll
};