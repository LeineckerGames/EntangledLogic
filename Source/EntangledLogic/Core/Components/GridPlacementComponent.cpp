#include "GridPlacementComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"

UGridPlacementComponent::UGridPlacementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGridPlacementComponent::BeginPlay()
{
	Super::BeginPlay();

	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	GridSize = GridPlacement->GetGridSize();
	
}


// Called every frame
void UGridPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

FVector UGridPlacementComponent::GetPlacementOffset() const
{
	return PlacementOffset;
}

int32 UGridPlacementComponent::GetFactorySize() const
{
	return FactorySize;
}

TArray<bool> UGridPlacementComponent::GetFactoryShape() const
{
	return FactoryShape;
}