#pragma once

#include "CoreMinimal.h"
#include "Wire.h"
#include "TestingWire.generated.h"

class AWireSegment; // Forward Declaration

UCLASS()
class ENTANGLEDLOGIC_API ATestingWire : public AWire
{
	GENERATED_BODY()
	
public:	
	ATestingWire();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	virtual void Tick(float DeltaTime) override;

	// Input Output Interface
	virtual void ConnectAllInputsAndOutputs() override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;

	// Linked list pointers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing")
	ATestingWire* PreviousWire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing")
	ATestingWire* NextWire;

	// Which segment/path does this wire currently belong to?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Testing")
	AWireSegment* AssignedSegment;
};
