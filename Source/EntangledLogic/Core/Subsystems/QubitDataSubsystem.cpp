#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "EntangledLogic/Core/DevSettings/FactorySettings.h"

using namespace qpp;

const int32 MAX_ENTANGLEMENT = 2;

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

// can desync entanglement groups, use only for initialization
void UQubitDataSubsystem::SetState(AQubit& qubit, ENamedState namedState)
{
	qubit.State->StateVector = GetStateAsVector(namedState);
	qubit.UpdateMeshData();
}

void UQubitDataSubsystem::Apply(AQubit& qubit, EQuantumGate gate)
{
	unsigned long LongEntPos = static_cast<unsigned long>(qubit.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);
	
	qubit.State->StateVector = apply(qubit.State->StateVector, gateMatrix, { LongEntPos });
	// if aliasing becomes an issue, try this instead:
	// state->X = qpp::apply(state->X.eval(), gateMatrix, { LongEntPos });

	qubit.UpdateMeshData();
}

void UQubitDataSubsystem::ApplyControlled(AQubit& control, AQubit& target, EQuantumGate gate)
{
	// if target and control are not already entangled, create a shared state
	CombineState(target, control);

	unsigned long LongEntPosT = static_cast<unsigned long>(target.EntanglementPosition);
	unsigned long LongEntPosC = static_cast<unsigned long>(control.EntanglementPosition);
	cmat gateMatrix = GetGateMatrix(gate);

	target.State->StateVector = applyCTRL(target.State->StateVector, gateMatrix, { LongEntPosC }, { LongEntPosT });

	// check disentanglement - currently assumes at most 2-qubit entanglement
	if (entanglement(target.State->StateVector) == 0)
	{
		cmat rho1 = ptrace1(target.State->StateVector);
		cmat rho2 = ptrace2(target.State->StateVector);
		
		control.State = MakeShared<FQubitData>();
		control.State->StateVector = rho2pure(rho1);
		control.State->qubits.Add(&control);
		control.EntanglementPosition = 0;

		target.State->StateVector = rho2pure(rho2);
		target.State->qubits.RemoveSingle(&control);
		target.EntanglementPosition = 0;

		control.State->UpdateQubitEntanglmentSplines();
	}

	control.UpdateMeshData();
	target.UpdateMeshData();
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

	// update state and entanglement position of B's siblings
	for (AQubit* q : qubitB.State->qubits)
	{
		q->EntanglementPosition += aLen;
		q->State = qubitA.State;
		UE_LOG(LogTemp, Display, TEXT("Adding qubit to entanglment array"))
		qubitA.State->qubits.Add(q);
		qubitA.State->UpdateQubitEntanglmentSplines();
	}

	return true;
}

// Destroys this qubit and all qubits entangled with it
// Todo: convert pointers outside of state structs into weak object pointers?
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