

#pragma once

#include "CoreMinimal.h"
#include "../FactoryBase.h"
#include "CNOTGateFactory.generated.h"

/**
 * 
 */
UCLASS()
class ENTANGLEDLOGIC_API ACNOTGateFactory : public AFactoryBase
{
	GENERATED_BODY()
	
	static constexpr int32 NUM_QUBIT_SLOTS = 2;

	int32 GetNumQubitSlots() override { return NUM_QUBIT_SLOTS; };
};
