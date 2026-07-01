#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WireSubsystem.generated.h"

class ATestingWire;
class AWireSegment;

UCLASS()
class ENTANGLEDLOGIC_API UWireSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Shapez.io Logic: Core operations
	UFUNCTION(BlueprintCallable, Category = "Wire System")
	void AddWireToPaths(ATestingWire* NewWire);

	UFUNCTION(BlueprintCallable, Category = "Wire System")
	void RemoveWireFromPath(ATestingWire* RemovedWire);

protected:
	// A list of all active continuous paths in the world
	UPROPERTY()
	TArray<AWireSegment*> ActiveSegments;

	// Helper function to spin up a brand new segment
	AWireSegment* CreateNewSegment(ATestingWire* StartWire);
};


