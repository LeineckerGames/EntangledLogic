#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FactoryInteractionInterface.generated.h"

enum class EPlacementMode : uint8;
UINTERFACE(MinimalAPI)
class UFactoryInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class ENTANGLEDLOGIC_API IFactoryInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginHover(EPlacementMode PlacementMode);
	virtual void EndHover(EPlacementMode PlacementMode);

	virtual void Interact(EPlacementMode PlacementMode);

};
