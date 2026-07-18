#pragma once

#include "EntangledLogic/Core/Framework/ProgressionGoalsDataStructs.h"
#include "PersistantStats.generated.h"

USTRUCT(BlueprintType)
struct FPersistantStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GnomesMurdered;
	
	UPROPERTY()
	TArray<FProgressionGoal> CurrentProgressionGoals;

	UPROPERTY()
	float SpeedOfItemOnWire;

};
