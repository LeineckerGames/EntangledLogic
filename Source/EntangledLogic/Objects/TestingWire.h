#pragma once

#include "CoreMinimal.h"
#include "Wire.h"
#include "Components/SplineComponent.h"
#include "TestingWire.generated.h"

// 1. A simple struct to hold our Factory-style data
USTRUCT(BlueprintType)
struct FWireItemData
{
	GENERATED_BODY()

	// The distance between this item and the item directly in front of it 
	// Defaulted to 20.0f (0.2 meters)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GapToNextItem = 20.0f; 

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* ItemMesh = nullptr;
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

	// Array of items currently traveling on this wire segment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	TArray<FWireItemData> ItemsOnWire;

	// The distance from the front-most item (Index 0) to the very end of the spline
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	float HeadGap;

	// The minimum distance required between items so they don't visually overlap
	// Set to 20.0f to match 0.2 meter wide items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	float ItemSize = 20.0f;

	// Units per second the items move 
	// 100.0f = 1 meter per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	float MovementSpeed = 100.0f;

	// The index of the first item gap that needs to compress when the wire is blocked
	int32 ActiveGapIndex = 1; 

	// Is the front of this wire currently blocked?
	bool bIsFrontBlocked = false;

	// Can items freely fall off the end of this belt?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wire Data")
	bool bCanOutput = false;

	// Call this function via Blueprints (or C++) to feed an item onto the start of the belt
	UFUNCTION(BlueprintCallable)
	void AddItemToWire(UStaticMesh* MeshToUse);

	// Removes the front-most item from the wire
	UFUNCTION(BlueprintCallable)
	void RemoveFrontItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	UStaticMesh* TestingItemMesh;

	void AddTestingItemToWire();

	// Toggles the output state when pressing U
	UFUNCTION(BlueprintCallable)
	void ToggleOutput();
};
