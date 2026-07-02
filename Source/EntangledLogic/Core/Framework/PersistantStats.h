#pragma once

#include "PersistantStats.generated.h"

USTRUCT(BlueprintType)
struct FPersistantStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 GnomesMurdered;

	UPROPERTY()
	float SpeedOfItemOnWire;

};
