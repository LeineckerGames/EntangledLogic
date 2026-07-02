#pragma once

#include "CoreMinimal.h"
#include "GridStructs.generated.h"

USTRUCT()
struct FGridCoordinate
{
	GENERATED_BODY()

	UPROPERTY()
	int32 XCoordinate;
	
	UPROPERTY()
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
