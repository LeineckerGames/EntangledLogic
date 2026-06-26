#include "InputKeyBindingSlot.h"
#include "Components/InputKeySelector.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedActionKeyMapping.h"
#include "InputAction.h"

void UInputKeyBindingSlot::InitializeSlot(UInputAction* InAction, EPlayerMappableKeySlot InSlotType, FKey CurrentKey)
{
    AssignedAction = InAction;
    BindingSlotType = InSlotType;

    if (KeySelectorButton)
    {
        KeySelectorButton->SetSelectedKey(FInputChord(CurrentKey));
    }
}

void UInputKeyBindingSlot::NativeConstruct()
{
    Super::NativeConstruct();

    if (KeySelectorButton)
    {
        KeySelectorButton->OnKeySelected.AddDynamic(this, &UInputKeyBindingSlot::OnKeyBindingSelected);
    }
}

void UInputKeyBindingSlot::OnKeyBindingSelected(FInputChord SelectedKey)
{
    if (!AssignedAction) return;

    APlayerController* PC = GetOwningPlayer();
    ULocalPlayer* LocalPlayer = PC ? PC->GetLocalPlayer() : nullptr;
    if (!LocalPlayer) return;

    auto* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (!InputSubsystem) return;

    UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();
    if (!UserSettings) return;

    UEnhancedPlayerMappableKeyProfile* ActiveProfile = UserSettings->GetActiveKeyProfile();
    if (!ActiveProfile) return;

    // 1. CONFLICT SCANNING
    FKey NewRequestedKey = SelectedKey.Key;
    TArray<FName> ConflictingMappingNames;

    if (ActiveProfile->GetMappingNamesForKey(NewRequestedKey, ConflictingMappingNames) > 0)
    {
        for (const FName& ConflictingName : ConflictingMappingNames)
        {
            if (ConflictingName != AssignedAction->GetFName())
            {
                // Conflict detected
                UE_LOG(LogTemp, Warning, TEXT("Conflict: %s is already using the %s key!"),
                    *ConflictingName.ToString(),
                    *NewRequestedKey.GetDisplayName().ToString());
            }
        }
    }

    // 2. MODERN RE-MAPPING PIPELINE
    FMapPlayerKeyArgs Args;
    Args.MappingName = AssignedAction->GetFName();
    Args.NewKey = NewRequestedKey;
    Args.Slot = BindingSlotType; // Primary or Secondary

    // Container required to catch engine remapping errors (e.g. key unmappable)
    FGameplayTagContainer FailureReasons;

    // Call the correct native settings function
    UserSettings->MapPlayerKey(Args, FailureReasons);

    // 3. Flush to disk
    UserSettings->SaveSettings();
}