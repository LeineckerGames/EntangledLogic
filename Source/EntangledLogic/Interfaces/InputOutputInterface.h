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

	virtual void SetAllInputOutputsVisibility(bool isVisible) = 0;

	virtual TArray<class UFactoryInputComponent*> GetInputComponents() = 0;

	virtual TArray<class UFactoryOutputComponent*> GetOutputComponents() = 0;

	class UFactoryInputComponent* GetConnectedInputComponent(class UFactoryOutputComponent* OutputComponentToConnect);

	class UFactoryOutputComponent* GetConnectedOutputComponent(class UFactoryInputComponent* InputComponentToConnect);

	void ConnectInputComponent(class UFactoryInputComponent* InputComponentToConnect, bool ConnectPreviousFactory);

	void ConnectOutputComponent(class UFactoryOutputComponent* OutputComponentToConnect, bool ConnectPreviousFactory);

	virtual void ConnectAllInputsAndOutputs() = 0;

	virtual bool IsQubitSlotEmpty(int32 QubitSlotIndex);

	virtual void TransferQubit(class AQubit* QubitToTransfer, int32 QubitSlotIndex);

	virtual void ConnectAllInputs() = 0;

	virtual void ConnectAllOutputs() = 0;

	virtual void DisconnectAllInputsAndOutputs();
};
