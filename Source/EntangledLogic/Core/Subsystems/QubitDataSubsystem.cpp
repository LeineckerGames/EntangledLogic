#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "EntangledLogic/Core/DevSettings/FactorySettings.h"

using namespace qpp;

const int32 MAX_ENTANGLEMENT = 3;

// Create a qubit actor without spawning it in the world
AQubit* UQubitDataSubsystem::NewQubit(ENamedState namedState)
{
	AQubit* q = NewObject<AQubit>();
	if (q)
	{
		q->State->StateVector = GetStateAsVector(namedState);
		q->State->qubits.Add(q);
		q->State->UpdateQubitEntanglmentSplines();
		q->UpdateMeshData();
	}

	return q;
}

AQubit* UQubitDataSubsystem::NewQubit()
{
	return NewQubit(ENamedState::Zero);
}

// Spawn a new qubit actor at the specified world location 
AQubit* UQubitDataSubsystem::SpawnQubit(FVector SpawnLocation, ENamedState namedState)
{
	AQubit* q = NULL;

	const UFactorySettings* Settings = GetDefault<UFactorySettings>();
	if (Settings && Settings->QubitClass)
	{
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		q = GetWorld()->SpawnActor<AQubit>(Settings->QubitClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (q)
		{
			q->State->StateVector = GetStateAsVector(namedState);
			q->State->qubits.Add(q);
			q->UpdateMeshData();
		}
	}
	return q;
}

// Spawn a new |0> qubit actor at the specified world location
AQubit* UQubitDataSubsystem::SpawnQubit(FVector SpawnLocation)
{
	return SpawnQubit(SpawnLocation, ENamedState::Zero);
}

// Manually set a qubit's state vector
// can desync entanglement groups, use only for initialization
void UQubitDataSubsystem::SetState(AQubit& qubit, ENamedState namedState)
{
	qubit.State->StateVector = GetStateAsVector(namedState);
	qubit.UpdateMeshData();
}

// Apply the specified gate to a single qubit
void UQubitDataSubsystem::Apply(AQubit& qubit, EQuantumGate gate)
{
	if (!IsValid(&qubit)) return;

	unsigned long LongEntPos = static_cast<unsigned long>(qubit.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);
	
	qubit.State->StateVector = apply(qubit.State->StateVector, gateMatrix, { LongEntPos });
	// if aliasing becomes an issue, try this instead:
	// state->X = qpp::apply(state->X.eval(), gateMatrix, { LongEntPos });

	qubit.UpdateMeshData();
}

// Apply the specified gate to the target qubit, controlled by the control qubit
void UQubitDataSubsystem::ApplyControlled(AQubit& control, AQubit& target, EQuantumGate gate)
{
	// if target and control are not already entangled, create a shared state
	if (CombineState(target, control) == false) return;

	unsigned long LongEntPosT = static_cast<unsigned long>(target.EntanglementPosition);
	unsigned long LongEntPosC = static_cast<unsigned long>(control.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);

	target.State->StateVector = applyCTRL(target.State->StateVector, gateMatrix, { LongEntPosC }, { LongEntPosT });

	// check disentanglement

	unsigned long StateSize = static_cast<unsigned long>(target.State->qubits.Num());
	int32 n = target.State->StateVector.rows();
	std::vector<idx> Partition = {};

	// cursed bitmask math
	for (int i = 0; i < n; i++)
	{
		if ((i & (1 << LongEntPosC)) == 0) continue;
		if ((i & (1 << LongEntPosT)) != 0) continue;

		Partition = {};
		for (idx j = 0; j < StateSize; j++)
		{
			if ((i & (1 << j)) != 0)
			{
				Partition.push_back(j);
			}
		}

		double MutualInfo = qmutualinfo(prj(target.State->StateVector), Partition, complement(Partition, StateSize));

		if (MutualInfo == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("Disentangling qubits"))
			SeperateState(target, Partition);
			break;
		}
	}

	control.UpdateMeshData();
	target.UpdateMeshData();
}

void UQubitDataSubsystem::SeperateState(AQubit& qubit, std::vector<idx> Partition)
{
	TSharedRef<FQubitData> OldState = qubit.State;
	std::vector<idx> Complement = complement(Partition, qubit.State->qubits.Num());
	cmat rhoC = ptrace(OldState->StateVector, Partition);
	cmat rhoP = ptrace(OldState->StateVector, Complement);

	TSharedRef<FQubitData> StateP = MakeShared<FQubitData>();
	TSharedRef<FQubitData> StateC = MakeShared<FQubitData>();
	StateP->StateVector = rho2pure(rhoP);
	StateC->StateVector = rho2pure(rhoC);

	int j = 0;
	for (idx i : Partition)
	{
		UE_LOG(LogTemp, Display, TEXT("Adding qubit %d to partition"), i)
			AQubit* q = OldState->qubits[i];
		q->State = StateP;
		q->EntanglementPosition = j++;
		StateP->qubits.Add(q);
	}

	j = 0;
	for (idx i : Complement)
	{
		UE_LOG(LogTemp, Display, TEXT("Adding qubit %d to complement"), i)
			AQubit* q = OldState->qubits[i];
		q->State = StateC;
		q->EntanglementPosition = j++;
		StateC->qubits.Add(q);
	}

	/*
	int32 j = 0;
	for (idx i : Complement)
	{
		target.State->qubits[i]->EntanglementPosition = j++;
	}

	j = 0;
	for (idx i : Partition)
	{
		target.State->qubits[i]->State = StateP;
		target.State->qubits[i]->EntanglementPosition = j++;
		StateP->qubits.Add(target.State->qubits[i]);
	}

	for (AQubit* q : StateP->qubits) {
		target.State->qubits.Remove(q);
	}*/

	StateP->UpdateQubitEntanglmentSplines();
	StateC->UpdateQubitEntanglmentSplines();
}

// take two qubits and combine their states into one common state
// returns false iff entranglement exceeds the set entanglement limit
bool UQubitDataSubsystem::CombineState(AQubit& qubitA, AQubit& qubitB)
{
	if (qubitA.State == qubitB.State) return true;

	if (qubitA.State->qubits.Num() + qubitB.State->qubits.Num() > MAX_ENTANGLEMENT)
	{
		return false;
	}

	int aLen = qubitA.State->qubits.Num();

	// create the new state vector via tensor product
	qubitA.State->StateVector = kron(qubitA.State->StateVector, qubitB.State->StateVector);
	for (AQubit* q : qubitA.State->qubits)
	{
		q->ToggleEntanglementFX(true);
	}

	// update state and entanglement position of B's siblings
	for (AQubit* q : qubitB.State->qubits)
	{
		q->EntanglementPosition += aLen;
		q->State = qubitA.State;
		UE_LOG(LogTemp, Display, TEXT("Adding qubit to entanglment group"))
		qubitA.State->qubits.Add(q);
		qubitA.State->UpdateQubitEntanglmentSplines();
		q->ToggleEntanglementFX(true);
	}
	return true;
}

// Destroys this qubit and all qubits entangled with it
void UQubitDataSubsystem::DeleteQubit(AQubit& qubit)
{
	if (&qubit)
	{
		for (AQubit* q : qubit.State->qubits)
		{
			if (q) q->Destroy();
			q = NULL;
		}
	}
}

// Returns the 
// curently only works for unentangled qubits
FVector UQubitDataSubsystem::GetBlochVector(AQubit& qubit)
{
	if (qubit.State->qubits.Num() == 1) {
		cmat rho = qpp::prj(qubit.State->StateVector);
		std::vector<double> x = qpp::rho2bloch(rho);
		return FVector(x[0], x[1], x[2]);
	}
	return FVector(0, 0, 1);
}

// Returns the state vector form of a named state
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

// Returns the density matrix form of a named state
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
	case EQuantumGate::Identity: return gt.Id();
	case EQuantumGate::X_Gate: return gt.X;
	case EQuantumGate::Y_Gate: return gt.Y;
	case EQuantumGate::Z_Gate: return gt.Z;
	case EQuantumGate::H_Gate: return gt.H;
	case EQuantumGate::S_Gate: return gt.S;
	case EQuantumGate::T_Gate: return gt.T;
	}

	return gt.Id();
}