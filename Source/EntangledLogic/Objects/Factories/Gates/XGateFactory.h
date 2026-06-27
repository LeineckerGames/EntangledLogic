#pragma once

#include "CoreMinimal.h"
#include "../FactoryBase.h"
#include "XGateFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API AXGateFactory : public AFactoryBase
{
	GENERATED_BODY()

public:

	AXGateFactory();

protected:

	UPROPERTY(EditAnywhere)
	AActor* InputSlot;

	UPROPERTY(EditAnywhere)
	AActor* OutputSlot;

	virtual void Destroyed() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;

};
