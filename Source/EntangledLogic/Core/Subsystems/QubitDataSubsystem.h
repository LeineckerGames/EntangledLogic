#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QppPlugin.h"
#include "QubitDataSubsystem.generated.h"

enum class EQuantumGate : uint8;
enum class ENamedState : uint8;

class AQubit;

UCLASS()
class ENTANGLEDLOGIC_API UQubitDataSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	AQubit* NewQubit();

	AQubit* NewQubit(ENamedState namedState);

	void SetState(AQubit& qubit, ENamedState namedState);

	void Apply(AQubit& qubit, EQuantumGate gate);

	void ApplyControlled(AQubit& control, AQubit& target, EQuantumGate gate);

	bool CombineState(AQubit& qubitA, AQubit& qubitB);

private:
	qpp::ket GetStateAsVector(ENamedState state);

	qpp::cmat GetStateAsMatrix(ENamedState state);

	qpp::cmat GetGateMatrix(EQuantumGate gate);

};
