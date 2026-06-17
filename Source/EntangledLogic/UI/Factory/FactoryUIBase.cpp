#include "FactoryUIBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Styling/SlateTypes.h"
#include "Components/NamedSlot.h" 

void UFactoryUIBase::NativeConstruct()
{
    Super::NativeConstruct();

    //HeaderText->SetText(FText::FromString("Testing Title"));

    if (PinToggleButton)
    {
        //UE_LOG(LogTemp, Display, TEXT("Binded Pin Button"));
        PinToggleButton->OnClicked.AddDynamic(this, &UFactoryUIBase::PinToggle);
    }
}

void UFactoryUIBase::PinToggle()
{
    if (isPinned)
    {
        PinToggleButton->SetStyle(PinOffButtonStyle);
        isPinned = false;
    }
    else
    {
        PinToggleButton->SetStyle(PinOnButtonStyle);
        isPinned = true;
    }
}

void UFactoryUIBase::SetHeaderText(FString TextToSet)
{
    HeaderText->SetText(FText::FromString(TextToSet));
}

bool UFactoryUIBase::GetIsPinned() const
{
    return isPinned;
}