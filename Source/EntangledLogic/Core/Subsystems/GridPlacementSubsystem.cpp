#include "GridPlacementSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"

void UGridPlacementSubsystem::SetSelectedFactory(TSubclassOf<AActor> FactoryClass)
{
	SelectedFactoryClass = FactoryClass;
	SelectedFactory = SpawnActorToPlaceFromClass(SelectedFactoryClass);
	PlacementMode = EPlacementMode::Placing;
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

// Overload for Grid Arrays
void UGridPlacementSubsystem::SetPlacedPositionMap(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape, bool isPlacedToSet)
{
	int i = 0;
	for (FGridCoordinate GridCoord : GridLocations)
	{
		// Skips factory sections that are open
		if (FactoryShape[i] == true)
		{
			PlacedPositionMap.Emplace(GridCoord, isPlacedToSet);
			UE_LOG(LogTemp, Display, TEXT("Set to %d at coordinates X:%d, Y:%d"), isPlacedToSet, GridCoord.XCoordinate, GridCoord.YCoordinate);
		}
		i++;
	}
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

float UGridPlacementSubsystem::GetGridSize() const
{
	return GridSize;
}

FVector UGridPlacementSubsystem::GetGridLocation(FVector Location, FVector GridOffset) const
{
	float HalfGridSize = GridSize / 2;
	FVector GridPosition = (Location - FVector(HalfGridSize, HalfGridSize, 0.0f) - GridOffset) / GridSize;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Grid Position X:%f, Y:%f"), GridPosition.X, GridPosition.Y));
	return GridPosition;
}

FVector UGridPlacementSubsystem::GetWorldGridLocation(FVector Location, FVector GridOffset)
{
	float HalfGridSize = GridSize / 2;
	FVector GridPosition = GetGridLocation(Location, GridOffset);
	FVector WorldGridPosition = FVector(
		(FMath::RoundHalfToEven(GridPosition.X) * GridSize) + HalfGridSize,
		(FMath::RoundHalfToEven(GridPosition.Y) * GridSize) + HalfGridSize,
		GridPosition.Z) + GridOffset;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("World Grid Position X:%f, Y:%f"), WorldGridPosition.X, WorldGridPosition.Y));
	return WorldGridPosition;
}

void UGridPlacementSubsystem::MoveSelectedFactoryOnGrid(FVector Location)
{
	UGridPlacementComponent* GridPlacementComponent = SelectedFactory->GetComponentByClass<UGridPlacementComponent>();
	FVector PlacementOffset = GridPlacementComponent->GetPlacementOffset();
	FVector GridLocation = GetWorldGridLocation(Location, PlacementOffset);
	SelectedFactory->SetActorLocation(GridLocation);
}

void UGridPlacementSubsystem::PlaceSelectedActor()
{
	UGridPlacementComponent* GridPlacementComponent = SelectedFactory->GetComponentByClass<UGridPlacementComponent>();
	TArray<FGridCoordinate> GridLocations = GridComponentToCoordinates(GridPlacementComponent);
	bool DidCollide = CollisionCheck(GridLocations, GridPlacementComponent->GetFactoryShape());
	if (DidCollide == false)
	{
		UE_LOG(LogTemp, Display, TEXT("Placing Actor"));
		SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), true);
		SelectedFactory = SpawnActorToPlaceFromClass(SelectedFactoryClass);
	}
}

// Returns True if it collides with something
bool UGridPlacementSubsystem::CollisionCheck(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape)
{
	int i = 0;
	for (FGridCoordinate GridCoord : GridLocations)
	{
		// Skips factory sections that are open
		if (FactoryShape[i] == true)
		{
			bool IsPlacedPosition = GetPlacedPositionMap(GridCoord.XCoordinate, GridCoord.YCoordinate);
			if (IsPlacedPosition == true)
			{
				return true;
			}
		}
		i++;
	}

	// If no collisions are found return
	return false;
}

TArray<FGridCoordinate> UGridPlacementSubsystem::GridComponentToCoordinates(UGridPlacementComponent* GridPlacementComponent)
{
	TArray<FGridCoordinate> GridLocations;
	// Get Factory Size and its Grid Coordinate
	int32 FactorySize = GridPlacementComponent->GetFactorySize();
	FVector GridPlacementComponentLocation = GridPlacementComponent->GetComponentTransform().GetLocation();

	//UE_LOG(LogTemp, Display, TEXT("GridPlacementComponentLocation: X = %f, Y = %f, Z = %f")
	//	, GridPlacementComponentLocation.X, GridPlacementComponentLocation.Y, GridPlacementComponentLocation.Z);

	FVector GridLocation = GetGridLocation(GridPlacementComponentLocation, FVector(0,0,0));
	int32 RoundedX = FMath::FloorToInt(GridLocation.X - 0.5f);
	int32 RoundedY = FMath::FloorToInt(GridLocation.Y - 0.5f);
	//UE_LOG(LogTemp, Display, TEXT("RoundedGridLocation: X = %d, Y = %d"), RoundedX, RoundedY);
	FGridCoordinate GridCoordinate = FGridCoordinate(RoundedX, RoundedY);
	int count = 0;
	for (int i = 0; i < FactorySize; i++)
	{
		for (int j = 0; j < FactorySize; j++)
		{
			FGridCoordinate AdjustedGridCoordinate = FGridCoordinate(GridCoordinate.XCoordinate - i, GridCoordinate.YCoordinate + j);
			//UE_LOG(LogTemp, Display, TEXT("[%d] Grid Coordinate: X = %d, Y = %d"), count, AdjustedGridCoordinate.XCoordinate, AdjustedGridCoordinate.YCoordinate);
			GridLocations.Add(AdjustedGridCoordinate);
			count++;
		}
	}
	return GridLocations;
}