#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputOutputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputOutputInterface : public UInterface
{
	GENERATED_BODY()
};

class ENTANGLEDLOGIC_API IInputOutputInterface
{
	GENERATED_BODY()

public:

	virtual void ConnectAllInputsAndOutputs() = 0;

	virtual void ConnectAllInputs() = 0;

	virtual void ConnectAllOutputs() = 0;
};
