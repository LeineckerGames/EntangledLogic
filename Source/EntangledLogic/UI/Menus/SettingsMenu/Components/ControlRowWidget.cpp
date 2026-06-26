#include "ControlRowWidget.h"
#include "Components/TextBlock.h"
#include "InputKeyBindingSlot.h"

void UControlRowWidget::InitializeRow(FText ActionName, UInputAction* InAction, FKey PrimaryKey, FKey SecondaryKey)
{

    if (ActionLabelText)
    {
        ActionLabelText->SetText(ActionName);
    }

    // Initialize the primary mapping slot button
    if (PrimaryKeySlot)
    {
        PrimaryKeySlot->InitializeSlot(InAction, EPlayerMappableKeySlot::First, PrimaryKey);
    }

    // Initialize the secondary mapping slot button
    if (SecondaryKeySlot)
    {
        SecondaryKeySlot->InitializeSlot(InAction, EPlayerMappableKeySlot::Second, SecondaryKey);
    }
}