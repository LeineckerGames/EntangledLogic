


#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"

void UQubitDataSubsystem::Apply(EOneQubitGate gate, AQubit& qubit)
{
	if (!qubit.State.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("Stateless qubit - cannot apply gate"));
		return;
	}

	unsigned long LongEntPos = static_cast<unsigned long>(qubit.EntanglementPosition);
	qpp::cmat gateMatrix = GetGateMatrix(gate);
	FQubitData* state = qubit.State.Get();

	state->DensityMatrix = qpp::apply(state->DensityMatrix, gateMatrix, { LongEntPos });
	// if aliasing becomes an issue, try this instead:
	// state->DensityMatrix = qpp::apply(state->DensityMatrix.eval(), gateMatrix, { LongEntPos });
}

void UQubitDataSubsystem::Apply(ETwoQubitGate gate, AQubit& qubitA, AQubit& qubitB)
{
	if (!qubitA.State.IsValid() || !qubitB.State.IsValid())
	{
		UE_LOG(LogTemp, Display, TEXT("Stateless qubit - cannot apply gate"));
		return;
	}

	qpp::cmat gateMatrix = GetGateMatrix(gate);
	unsigned long LongEntPosA = static_cast<unsigned long>(qubitA.EntanglementPosition);
	unsigned long LongEntPosB = static_cast<unsigned long>(qubitB.EntanglementPosition);


	// todo: check disentanglement - requires finding all entangled AQubits


}

// convert gate enum to qpp matrix
qpp::cmat UQubitDataSubsystem::GetGateMatrix(EOneQubitGate gate)
{
	switch (gate) {
	case EOneQubitGate::Identity : return qpp::gt.Id();
	case EOneQubitGate::X_Gate : return qpp::gt.X;
	case EOneQubitGate::Y_Gate : return qpp::gt.Y;
	case EOneQubitGate::Z_Gate : return qpp::gt.Z;
	case EOneQubitGate::H_Gate : return qpp::gt.H;
	}

	return qpp::cmat();
}

qpp::cmat UQubitDataSubsystem::GetGateMatrix(ETwoQubitGate gate)
{
	switch (gate) {
	case ETwoQubitGate::Identity: return qpp::gt.Id2;
	case ETwoQubitGate::CNOT_Gate: return qpp::gt.CNOT;
	}

	return qpp::cmat();
}