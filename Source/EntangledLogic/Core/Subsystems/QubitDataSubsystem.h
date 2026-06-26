#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QppPlugin.h"
#include "QubitDataSubsystem.generated.h"

enum class EOneQubitGate : uint8;
enum class ETwoQubitGate : uint8;
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

	void Apply(AQubit& qubit, EOneQubitGate gate);

	void Apply(AQubit& qubitA, AQubit& qubitB, ETwoQubitGate gate);

private:
	qpp::cmat GetNamedState(ENamedState state);

	qpp::cmat GetGateMatrix(EOneQubitGate gate);

	qpp::cmat GetGateMatrix(ETwoQubitGate gate);

};
