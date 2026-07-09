#pragma once

#include "CoreMinimal.h"
#include "FactoryBase.h"
#include "BlackholeFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API ABlackholeFactory : public AFactoryBase
{
	GENERATED_BODY()

	static constexpr int32 NUM_QUBIT_SLOTS = 4;

	int32 GetNumQubitSlots() override { return NUM_QUBIT_SLOTS; };

public:

	ABlackholeFactory();

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnFactoryTick() override;

public:

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;

};

