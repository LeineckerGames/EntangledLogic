#pragma once

#include "CoreMinimal.h"
#include "FactoryBase.h"
#include "QubitGeneratorFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AQubitGeneratorFactory : public AFactoryBase
{
	GENERATED_BODY()

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	virtual void OnFactoryTick() override;

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;

};
