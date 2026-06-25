


#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"

using namespace qpp;

AQubit* UQubitDataSubsystem::NewQubit(ENamedState namedState)
{
	AQubit* q = NewObject<AQubit>();
	if (q)
	{
		q->State->DensityMatrix = GetNamedState(namedState);
		q->State->qubits.Add(q);
	}

	return q;
}

AQubit* UQubitDataSubsystem::NewQubit()
{
	return NewQubit(ENamedState::Zero);
}

// can desync entanglement groups, use with caution
void UQubitDataSubsystem::SetState(AQubit& qubit, ENamedState namedState)
{
	qubit.State->DensityMatrix = GetNamedState(namedState);
}

void UQubitDataSubsystem::Apply(EOneQubitGate gate, AQubit& qubit)
{
	if (false)
	{
		UE_LOG(LogTemp, Display, TEXT("Stateless qubit - cannot apply gate"));
		return;
	}

	unsigned long LongEntPos = static_cast<unsigned long>(qubit.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);
	FQubitData* state = &qubit.State.Get();

	state->DensityMatrix = apply(state->DensityMatrix, gateMatrix, { LongEntPos });
	// if aliasing becomes an issue, try this instead:
	// state->DensityMatrix = qpp::apply(state->DensityMatrix.eval(), gateMatrix, { LongEntPos });
}

void UQubitDataSubsystem::Apply(ETwoQubitGate gate, AQubit& qubitA, AQubit& qubitB)
{
	if (false)
	{
		UE_LOG(LogTemp, Display, TEXT("Stateless qubit - cannot apply gate"));
		return;
	}

	cmat gateMatrix = GetGateMatrix(gate);
	unsigned long LongEntPosA = static_cast<unsigned long>(qubitA.EntanglementPosition);
	unsigned long LongEntPosB = static_cast<unsigned long>(qubitB.EntanglementPosition);


	// todo: check disentanglement - requires finding all entangled AQubits
}


qpp::cmat UQubitDataSubsystem::GetNamedState(ENamedState state)
{
	switch (state)
	{
	case ENamedState::Zero:  return st.pz0;
	case ENamedState::One:   return st.pz1;
	case ENamedState::Plus:  return st.px0;
	case ENamedState::Minus: return st.px1;
	}

	return st.pz0;
}

// convert gate enum to qpp matrix
cmat UQubitDataSubsystem::GetGateMatrix(EOneQubitGate gate)
{
	switch (gate) {
	case EOneQubitGate::Identity : return gt.Id();
	case EOneQubitGate::X_Gate : return gt.X;
	case EOneQubitGate::Y_Gate : return gt.Y;
	case EOneQubitGate::Z_Gate : return gt.Z;
	case EOneQubitGate::H_Gate : return gt.H;
	}

	return gt.Id();
}

cmat UQubitDataSubsystem::GetGateMatrix(ETwoQubitGate gate)
{
	switch (gate) {
	case ETwoQubitGate::Identity: return gt.Id2;
	case ETwoQubitGate::CNOT_Gate: return gt.CNOT;
	}

	return gt.Id2;
}