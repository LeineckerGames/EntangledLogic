#pragma once

#include "CoreMinimal.h"
#include "QppPlugin.h"
#include "QubitDataStructs.generated.h"


USTRUCT()
struct FEntangledGroup
{
	GENERATED_USTRUCT_BODY()
	TArray<AQubit*> qubits;
};

USTRUCT()
struct FQubitData
{
	GENERATED_USTRUCT_BODY()
	qpp::cmat DensityMatrix;
	FEntangledGroup group;
};