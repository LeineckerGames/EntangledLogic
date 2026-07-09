#pragma once

#include "CoreMinimal.h"
#include "../FactoryBase.h"
#include "HGateFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AHGateFactory : public AFactoryBase
{
	GENERATED_BODY()
	
	static constexpr int32 NUM_QUBIT_SLOTS = 1;

	int32 GetNumQubitSlots() override { return NUM_QUBIT_SLOTS; };

public:

	AHGateFactory();

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnFactoryTick() override;

	virtual void OnQubitProcessed() override;

public:

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;

};
