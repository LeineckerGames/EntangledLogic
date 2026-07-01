

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WireSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ENTANGLEDLOGIC_API UWireSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void MoveOneSpace();

};


