
#include "GridPlacementSubsystem.h"

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