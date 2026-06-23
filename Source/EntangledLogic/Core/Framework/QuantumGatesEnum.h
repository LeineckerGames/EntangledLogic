#pragma once

UENUM()
enum class EOneQubitGate : uint8
{
	Identity,
	X_Gate,
	Y_Gate,
	Z_Gate,
	H_Gate,
	S_Gate // maybe
};

UENUM()
enum class ETwoQubitGate : uint8
{
	Identity,
	CNOT_Gate
};