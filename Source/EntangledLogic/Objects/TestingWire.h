#pragma once

#include "CoreMinimal.h"
#include "Wire.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "TestingWire.generated.h"

/**
 * 
 */
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

	UFUNCTION()
	void ProcessMovementTimeline(float Value);

	UFUNCTION()
	void OnEndMovementTimeline();

	// Function to start movement on key press
	UFUNCTION()
	void StartMovement();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spline")
	UCurveFloat* MovementCurve;

private:
	UPROPERTY()
	FTimeline MovementTimeline;
};
