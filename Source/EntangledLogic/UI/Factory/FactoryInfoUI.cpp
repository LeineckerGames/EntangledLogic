#include "FactoryInfoUI.h"
#include "FactoryUIBase.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/PanelSlot.h"
#include "EntangledLogic/UI/Factory/QubitDisplaySlot.h"

void UFactoryInfoUI::SetFactoryDescriptionText(FString FactoryDescription)
{
	FactoryDescriptionText->SetText(FText::FromString(*FactoryDescription));
}

void UFactoryInfoUI::SetFactoryInfoText(FString FactoryInfo)
{
	FactoryInfoText->SetText(FText::FromString(*FactoryInfo));
}

void UFactoryInfoUI::SetHeaderText(FString FactoryHeader)
{
	UIBase->SetHeaderText(FactoryHeader);
}

// create and initialize Qubit display slots
void UFactoryInfoUI::PopulateQubits(int32 NumQubits)
{
	if (QubitDisplayBox)
	{
		QubitDisplayBox->ClearChildren();

		// create a QubitDisplaySlot for each qubit
		for (int i = 0; i < NumQubits; i++)
		{
			UQubitDisplaySlot* QubitSlot = CreateWidget<UQubitDisplaySlot>(this, QubitDisplaySlotClass);
			FString name = FString("Qubit ").Append(FString::FromInt(i));
			if (QubitSlot)
			{
				QubitSlot->SetSlotName(name);
				QubitDisplayBox->AddChild(QubitSlot);

				// set up slot formatting
				UHorizontalBoxSlot* slot = Cast<UHorizontalBoxSlot>(QubitSlot->Slot);
				if (slot)
				{
					slot->SetSize(FSlateChildSize(ESlateSizeRule::Type::Fill));
					if (i == 0) slot->SetPadding(FMargin(3));
					else slot->SetPadding(FMargin(0, 3, 3, 3));
				}
			}
		}
	}
}

// Set the qubit data of the QubitDisplay at specified index
void UFactoryInfoUI::UpdateQubit(int32 index, AQubit &Q)
{
	if (QubitDisplayBox)
	{
		UQubitDisplaySlot* QubitSlot = Cast<UQubitDisplaySlot>(QubitDisplayBox->GetChildAt(index));
		if (QubitSlot) QubitSlot->SetQubit(Q);
	}
}

bool UFactoryInfoUI::GetIsPinned() const
{
	return UIBase->GetIsPinned();
}