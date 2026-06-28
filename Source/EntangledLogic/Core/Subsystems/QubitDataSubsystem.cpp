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
	qubit.State->bIsDensityMatrix = false;
}

void UQubitDataSubsystem::Apply(AQubit& qubit, EQuantumGate gate)
{
	unsigned long LongEntPos = static_cast<unsigned long>(qubit.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);
	FQubitData* state = &qubit.State.Get();

	if (qubit.State->bIsDensityMatrix) {
		state->DensityMatrix = apply(state->DensityMatrix, gateMatrix, { LongEntPos });
	}
	else
	{
		state->StateVector = apply(state->StateVector, gateMatrix, { LongEntPos });
	}
	
	// if aliasing becomes an issue, try this instead:
	// state->X = qpp::apply(state->X.eval(), gateMatrix, { LongEntPos });
}

void UQubitDataSubsystem::ApplyControlled(AQubit& target, AQubit& control, EQuantumGate gate)
{
	cmat gateMatrix = GetGateMatrix(gate);
	unsigned long LongEntPosA = static_cast<unsigned long>(target.EntanglementPosition);
	unsigned long LongEntPosB = static_cast<unsigned long>(control.EntanglementPosition);


	// todo: check disentanglement - requires finding all entangled AQubits
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