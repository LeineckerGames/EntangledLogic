

#pragma once

#include "CoreMinimal.h"
#include "../FactoryBase.h"
#include "XGateFactory.generated.h"

/**
 * 
 */
UCLASS()
class ENTANGLEDLOGIC_API AXGateFactory : public AFactoryBase
{
	GENERATED_BODY()

public:

	AXGateFactory();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneCaptureComponent2D* FactoryInsideCamera;

};
