

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FactoryVariantInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFactoryVariantInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for factories that provide swap/variant behaviour when "rotated".
 * Implement this on factory actors that want rotation to cycle between different actor classes.
 */
class ENTANGLEDLOGIC_API IFactoryVariantInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Return the next factory class variant given the current factory class.
	// Implementations should own the ordered list of variants and return the next one in sequence.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "FactoryVariant")
	TSubclassOf<AActor> GetNextVariant(TSubclassOf<AActor> CurrentClass);
};
