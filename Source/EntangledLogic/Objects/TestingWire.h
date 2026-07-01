#pragma once

#include "CoreMinimal.h"
#include "Wire.h"
#include "TestingWire.generated.h"

UCLASS()
class ENTANGLEDLOGIC_API ATestingWire : public AWire
{
	GENERATED_BODY()
	
public:	
	ATestingWire();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Linked list pointers essential for creating the WireSegment path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing", meta = (AllowPrivateAccess = "true"))
	ATestingWire* PreviousWire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing", meta = (AllowPrivateAccess = "true"))
	ATestingWire* NextWire;
};
