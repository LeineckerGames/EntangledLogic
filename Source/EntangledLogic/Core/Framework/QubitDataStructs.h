#pragma once

#include "CoreMinimal.h"
#include "QppPlugin.h"
#include "QubitDataStructs.generated.h"


USTRUCT()
struct FQubitData
{
	GENERATED_USTRUCT_BODY()

	int8 size;
	qpp::cmat DensityMatrix;
};