#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "WireSegment.generated.h"

// Forward declaration
class ATestingWire;
class AQubit;

USTRUCT(BlueprintType)
struct FWireItemData
{
	GENERATED_BODY()

	// The distance between this item and the item directly in front of it 
	// Defaulted to 25.0f (0.25 meters)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GapToNextItem = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AQubit* QubitData = nullptr;

	// Add UpdateMaterial function
	// Material instance
};

UCLASS()
class ENTANGLEDLOGIC_API AWireSegment : public AActor
{
	GENERATED_BODY()

public:
	AWireSegment();

protected:
	virtual void BeginPlay() override;

	bool IsQubitAtEndOfSpline() const;

public:
	virtual void Tick(float DeltaTime) override;

	void OnFactoryTick();

	void OutputQubits();

	// Initializes the spline path from a linked list of wires
	UFUNCTION(BlueprintCallable, Category = "Wire Segment")
	void InitializeSegment(ATestingWire* StartWire);

	// Attempt to push an item onto the start of the queue
	UFUNCTION(BlueprintCallable, Category = "Wire Segment")
	bool AddItemToWire(AQubit* QubitData);

	// Removes the front-most item from the queue
	UFUNCTION(BlueprintCallable, Category = "Wire Segment")
	AQubit* RemoveFrontItem();

	// Called when an item reaches the end of the wire segment path
	UFUNCTION(BlueprintCallable, Category = "Wire Segment")
	bool LeaveWireSegment();

	UFUNCTION(BlueprintPure, Category = "Wire Segment")
	bool IsEmpty();

	UFUNCTION(BlueprintPure, Category = "Wire Segment")
	bool IsFull();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

	// Subsystem Data
	UPROPERTY(BlueprintReadOnly, Category = "Wire Segment")
	ATestingWire* StartWire;

	UPROPERTY(BlueprintReadOnly, Category = "Wire Segment")
	ATestingWire* EndWire;

	// Queue Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	TArray<FWireItemData> ItemsOnWire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	float HeadGap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	float ItemSize = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	float SingleWireLength = 100.0f; // A single wire tile is this many cm across.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	FVector SplineOffset = FVector(0.0f, 0.0f, 20.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	float MovementSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	int Capacity = 0; // 0 implies infinite within length limits

	int32 ActiveGapIndex = 1;
	bool bIsFrontBlocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* TestingItemMesh;

	UFUNCTION()
	void AddTestingItemToWire(AQubit* QubitData);

	UFUNCTION()
	void AddWireToEndOfSegment(ATestingWire* WireToAdd);

	UFUNCTION()
	void AddWireToStartOfSegment(ATestingWire* WireToAdd);

	UFUNCTION()
	void RemoveWireFromEndOfSegment(ATestingWire* WireToRemove);

	// Remove the wire at the start of the segment (first spline point).
	UFUNCTION()
	void RemoveWireFromStartOfSegment(ATestingWire* WireToRemove);

	UFUNCTION()
	TArray<FWireItemData> RemoveWireFromMiddleOfSegment(ATestingWire* WireToRemove);

	UFUNCTION()
	AQubit* RemoveQubitAtIndex(int32 Index);

	/*
	UFUNCTION()
	TArray<float> GetInputKeysSandwichingWire(ATestingWire* WireToRemove);
	*/
};