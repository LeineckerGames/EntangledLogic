#pragma once

#include "CoreMinimal.h"
#include "GridStructs.h"
#include "SaveDataStructs.generated.h"

USTRUCT()
struct FFactorySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FGridCoordinate GridLocation;

	UPROPERTY()
	TSubclassOf<AActor> FactoryClass;

	UPROPERTY()
	FTransform FactoryTransform;

	// Can prob remove this later
	UPROPERTY()
	FRotator FactoryRotation;

};