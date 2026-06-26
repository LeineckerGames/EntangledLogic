#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "SettingsDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FKeyMappingDisplayInfo
{
    GENERATED_BODY()

    // The user-friendly name displayed in the UI (e.g., "Move Forward")
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    FText ActionFriendlyName;

    // The actual Enhanced Input Action asset it maps to (e.g., IA_MoveForward)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
    UInputAction* TargetAction = nullptr;
};