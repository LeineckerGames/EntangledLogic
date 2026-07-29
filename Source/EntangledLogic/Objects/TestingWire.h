#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
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

	virtual void DisconnectAllInputsAndOutputs() override;

	virtual bool IsQubitSlotEmpty(int32 QubitSlotIndex) override;

	virtual void TransferQubit(class AQubit* QubitToTransfer, int32 QubitSlotIndex) override;

	virtual void ConnectAllInputs() override;

	virtual void ConnectAllOutputs() override;

	UFUNCTION()
	ATestingWire* GetInputWire(); // Returns null if the input is not a wire.

	UFUNCTION()
	ATestingWire* GetOutputWire();  // Returns null if the output is not a wire.

	// Which segment/path does this wire currently belong to?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Testing")
	AWireSegment* AssignedSegment;

	
	// The points for the spline to attach to, are attached to this component. Access them using:
	//		TArray<USceneComponent*> PointsArray;
	//		PointsRoot->GetChildrenComponents(true, PointsArray);
	// PointsArray stores the points, and now you can access a specific one using PointsArray[index]->GetComponentLocation();
	// Create new points in the Blueprint Editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing", meta = (AllowPrivateAccess = "true"))
	USceneComponent* PointsRoot;

	// Minimum required point; this is default first point, attached to the PointsRoot by default
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultPoint;
	

	UFUNCTION()
	FVector GetPointAtIndex(int32 i);


	// Qubits follow the path defined by this spline.
	// Splines are defined by points, as well as information on how to connect these points (e.g. tangents).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Testing", meta = (AllowPrivateAccess = "true"))
	USplineComponent* WireSpline;
};
