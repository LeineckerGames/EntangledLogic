


#include "WireSubsystem.h"
#include "EntangledLogic/Objects/Wire.h"

void UWireSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UGridPlacementSubsystem* GridPlacementSubsystem = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	if (GridPlacementSubsystem) 
	{
		GridPlacementSubsystem->OnBuildingPlacedDelegate.AddDynamic(this, &UWireSubsystem::OnBuildingPlaced);
	}
}

void UWireSubsystem::OnBuildingPlaced(AActor* PlacedActor) 
{
	if (PlacedActor) {
		AWire* PlacedWire = Cast<AWire>(PlacedActor);
		if (PlacedWire)
		{
			// The placed actor is a wire
			UE_LOG(LogTemp, Warning, TEXT("Wire placed: %s"), *PlacedWire->GetName());

			// Check for wires that are adjacent to the placed wire
			UGridPlacementSubsystem* GridPlacementSubsystem = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
		}
	}
	return;
}