#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KeyboardIconKeymappingsData.generated.h"

// Used for Player Controls UI
enum class EPlacementMode : uint8;
class UTexture2D;
USTRUCT(BlueprintType)
struct FControlData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UInputAction* InputAction;

	UPROPERTY(EditAnywhere)
	TMap<EPlacementMode, FText> PlacementModeDescriptions;
};

struct FKey;
UCLASS(BlueprintType)
class ENTANGLEDLOGIC_API UKeyboardIconKeymappingsData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Mappings|Icons")
	TMap<FKey, UTexture2D*> KeyIconMap;
	
	UPROPERTY(EditAnywhere, Category = "Mappings|Controls")
	TArray<FControlData> ControlsList;

};
