// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GridPlacementSubsystem.generated.h"

USTRUCT()
struct FGridCoordinate
{
	GENERATED_BODY()
	int32 XCoordinate;
	int32 YCoordinate;

	// Needed to use Struct within Key Value of TMap
	bool operator==(const FGridCoordinate& Other) const
	{
		return XCoordinate == Other.XCoordinate && YCoordinate == Other.YCoordinate;
	}
};

// Needed to use Struct within Key Value of TMap
inline uint32 GetTypeHash(const FGridCoordinate& Coordinate)
{
	// HashCombine merges the hashes of your individual variables
	return HashCombine(::GetTypeHash(Coordinate.XCoordinate), ::GetTypeHash(Coordinate.YCoordinate));
}

UCLASS()
class ENTANGLEDLOGIC_API UGridPlacementSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	float GridSize;

	TMap<FGridCoordinate, bool> PlacedPositionMap;
	
	void SetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition, bool isPlacedToSet);

	bool GetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition);
};
