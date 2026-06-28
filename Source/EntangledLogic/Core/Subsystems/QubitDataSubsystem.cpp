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
		q->State->StateVector = GetStateAsVector(namedState);
		q->State->qubits.Add(q);
	}

	return q;
}

AQubit* UQubitDataSubsystem::NewQubit()
{
	return NewQubit(ENamedState::Zero);
}

// can desync entanglement groups, use only for initialization
void UQubitDataSubsystem::SetState(AQubit& qubit, ENamedState namedState)
{
	qubit.State->StateVector = GetStateAsVector(namedState);
}

void UQubitDataSubsystem::Apply(AQubit& qubit, EQuantumGate gate)
{
	unsigned long LongEntPos = static_cast<unsigned long>(qubit.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);
	
	qubit.State->StateVector = apply(qubit.State->StateVector, gateMatrix, { LongEntPos });
	// if aliasing becomes an issue, try this instead:
	// state->X = qpp::apply(state->X.eval(), gateMatrix, { LongEntPos });
}

void UQubitDataSubsystem::ApplyControlled(AQubit& target, AQubit& control, EQuantumGate gate)
{
	// if target and control are not already entangled, create a shared state
	CombineState(target, control);

	unsigned long LongEntPosT = static_cast<unsigned long>(target.EntanglementPosition);
	unsigned long LongEntPosC = static_cast<unsigned long>(control.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);

	target.State->StateVector = applyCTRL(target.State->StateVector, gateMatrix, { LongEntPosC }, { LongEntPosT });

	// todo: check disentanglement
}

void UQubitDataSubsystem::CombineState(AQubit& qubitA, AQubit& qubitB)
{
	if (qubitA.State == qubitB.State) return;

	int aLen = qubitA.State->qubits.Num();

	// create the new state vector via tensor product
	qubitA.State->StateVector = kron(qubitA.State->StateVector, qubitB.State->StateVector);

	// update state and entanglement position of B's siblings
	for (AQubit* q : qubitB.State->qubits)
	{
		q->EntanglementPosition += aLen;
		q->State = qubitA.State;
		qubitA.State->qubits.Add(q);
	}
}

qpp::ket UQubitDataSubsystem::GetStateAsVector(ENamedState state)
{
	switch (state)
	{
	case ENamedState::Zero:  return st.z0;
	case ENamedState::One:   return st.z1;
	case ENamedState::Plus:  return st.x0;
	case ENamedState::Minus: return st.x1;
	}

	return st.z0;
}


qpp::cmat UQubitDataSubsystem::GetStateAsMatrix(ENamedState state)
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
cmat UQubitDataSubsystem::GetGateMatrix(EQuantumGate gate)
{
	switch (gate) {
	case EQuantumGate::Identity : return gt.Id();
	case EQuantumGate::X_Gate : return gt.X;
	case EQuantumGate::Y_Gate : return gt.Y;
	case EQuantumGate::Z_Gate : return gt.Z;
	case EQuantumGate::H_Gate : return gt.H;
	}

	return gt.Id();
}