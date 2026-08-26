#pragma once

UENUM()
enum class EQuantumGate : uint8
{
	Identity,
	X_Gate,
	Y_Gate,
	Z_Gate,
	H_Gate,
	S_Gate, // maybe
	T_Gate
};

UENUM()
enum class ENamedState : uint8
{
	Zero,
	One,
	Plus,
	Minus
};