#pragma once

#include "CoreMinimal.h"
#include "QppPlugin.h"
#include "QubitDataStructs.generated.h"


USTRUCT()
struct FQubitData
{
	GENERATED_USTRUCT_BODY()

	bool bIsDensityMat;
	qpp::ket StateVector;
	qpp::cmat DensityMatrix;
};