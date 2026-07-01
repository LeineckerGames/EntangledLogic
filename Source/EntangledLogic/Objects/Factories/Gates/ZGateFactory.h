#pragma once

#include "CoreMinimal.h"
#include "../FactoryBase.h"
#include "ZGateFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AZGateFactory : public AFactoryBase
{
	GENERATED_BODY()

protected:

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;

};
