
#include "GridPlacementSubsystem.h"

void UGridPlacementSubsystem::SetSelectedFactory(TSubclassOf<AActor> FactoryClass)
{
	SelectedFactoryClass = FactoryClass;
	SelectedFactory = SpawnActorToPlaceFromClass(SelectedFactoryClass);



}

AActor* UGridPlacementSubsystem::SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor)
{
	FRotator SpawnRotation = FRotator::ZeroRotator;
	//Maybe change this to be 0,0,0 idk
	FVector SpawnLocation = FVector(0, 0, 0);

	FActorSpawnParameters SpawnParams;

	AActor* SelectedActorToPlace = GetWorld()->SpawnActor<AActor>(SelectedActor->GetDefaultObject()->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
	return SelectedActorToPlace;
}

void UGridPlacementSubsystem::SetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition, bool isPlacedToSet)
{
	FGridCoordinate GridCoordinate;
	GridCoordinate.XCoordinate = GridXPosition;
	GridCoordinate.YCoordinate = GridYPosition;
	PlacedPositionMap.Emplace(GridCoordinate, isPlacedToSet);
	UE_LOG(LogTemp, Display, TEXT("Set to %d at coordinates X:%d, Y:%d"), isPlacedToSet, GridXPosition, GridYPosition);
}

bool UGridPlacementSubsystem::GetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition)
{
	FGridCoordinate GridCoordinate;
	GridCoordinate.XCoordinate = GridXPosition;
	GridCoordinate.YCoordinate = GridYPosition;
	bool* isPlacedPointer;
	isPlacedPointer = PlacedPositionMap.Find(GridCoordinate);
	if (isPlacedPointer)
	{
		bool isPlaced = *isPlacedPointer;
		UE_LOG(LogTemp, Display, TEXT("%d is found at coordinates X:%d, Y:%d"), isPlaced, GridXPosition, GridYPosition);
		return isPlaced;
	}
	return false;
}

EPlacementMode UGridPlacementSubsystem::GetPlacementMode() const
{
	return PlacementMode;
}