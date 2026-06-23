

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "QppPlugin.h"
#include "QubitDataSubsystem.generated.h"

enum class EOneQubitGate : uint8;
enum class ETwoQubitGate : uint8;

UCLASS()
class ENTANGLEDLOGIC_API UQubitDataSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void Apply(EOneQubitGate gate, class AQubit& qubit);

	void Apply(ETwoQubitGate gate, class AQubit& qubitA, class AQubit& qubitB);

private:
	qpp::cmat GetGateMatrix(EOneQubitGate gate);

	qpp::cmat GetGateMatrix(ETwoQubitGate gate);

};
