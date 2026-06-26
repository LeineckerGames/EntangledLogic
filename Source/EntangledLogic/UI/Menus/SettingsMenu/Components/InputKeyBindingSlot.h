#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "InputKeyBindingSlot.generated.h"

class UInputAction;
class UInputKeySelector;

UCLASS()
class ENTANGLEDLOGIC_API UInputKeyBindingSlot : public UUserWidget
{
    GENERATED_BODY()

public:
    // Configures the slot with its target action and whether it's primary or secondary
    void InitializeSlot(UInputAction* InAction, EPlayerMappableKeySlot InSlotType, FKey CurrentKey);

protected:
    virtual void NativeConstruct() override;

    // UI Elements
    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* KeySelectorButton;

    // Functions
    UFUNCTION()
    void OnKeyBindingSelected(FInputChord SelectedKey);

private:
    UPROPERTY()
    UInputAction* AssignedAction = nullptr;

    // Tracks if this specific slot button is Primary (First) or Secondary (Second)
    EPlayerMappableKeySlot BindingSlotType = EPlayerMappableKeySlot::First;
};
