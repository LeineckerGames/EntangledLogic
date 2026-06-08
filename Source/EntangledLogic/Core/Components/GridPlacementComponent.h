

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GridPlacementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENTANGLEDLOGIC_API UGridPlacementComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridPlacementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Factory Shape Info
	UPROPERTY(EditAnywhere, Category = "Factory Shape")
	int32 FactorySize;
	UPROPERTY(EditAnywhere, Category = "Factory Shape")
	TArray<bool> FactoryShape;
	UPROPERTY(EditAnywhere, Category = "Factory Shape")
	FVector PlacementOffset;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
