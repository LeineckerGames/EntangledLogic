#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveMetadata.generated.h"

USTRUCT(BlueprintType)
struct FSlotMetadata
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	bool bIsSlotEmpty = true;

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	FString PlayerName = "Empty Slot";

	UPROPERTY(BlueprintReadOnly, Category = "SaveData")
	FString DateSaved = "";
};

UCLASS()
class ENTANGLEDLOGIC_API USaveMetadata : public USaveGame
{
	GENERATED_BODY()

public:
	// Maps a Slot Index (e.g., 1, 2, 3, 4) to its respective metadata
	UPROPERTY()
	TMap<int32, FSlotMetadata> SlotRegistry;
};