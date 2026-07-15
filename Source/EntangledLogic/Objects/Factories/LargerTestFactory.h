#pragma once

#include "CoreMinimal.h"
#include "FactoryBase.h"
#include "LargerTestFactory.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API ALargerTestFactory : public AFactoryBase
{
	GENERATED_BODY()

	static constexpr int32 NUM_QUBIT_SLOTS = 1;

	int32 GetNumQubitSlots() override { return NUM_QUBIT_SLOTS; };

protected:

	virtual void BeginPlay() override;
	
};
