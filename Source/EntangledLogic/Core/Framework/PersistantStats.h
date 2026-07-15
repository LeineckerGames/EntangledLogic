#pragma once

#include "EntangledLogic/Core/Framework/ProgressionGoals.h"
#include "PersistantStats.generated.h"

USTRUCT(BlueprintType)
struct FPersistantStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GnomesMurdered;
	
	UPROPERTY()
	EProgressionGoals CurrentProgressionGoal;

	UPROPERTY()
	int32 CurrentGoalAcceptedStatesCount = 0;

	UPROPERTY()
	float SpeedOfItemOnWire;

};
