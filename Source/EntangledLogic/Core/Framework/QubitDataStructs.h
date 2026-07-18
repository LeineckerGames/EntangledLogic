#pragma once

#include "CoreMinimal.h"
#include "QppPlugin.h"
#include "QubitDataStructs.generated.h"

USTRUCT()
struct FQubitData
{
	GENERATED_USTRUCT_BODY()

	//bool bIsDensityMatrix = false;
	//qpp::cmat DensityMatrix;

	//UPROPERTY()
	qpp::ket StateVector;

	UPROPERTY()
	TArray<AQubit*> qubits;
};