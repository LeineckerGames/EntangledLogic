#include "FactoryInfoUI.h"
#include "FactoryUIBase.h"
#include "Components/TextBlock.h"

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

bool UFactoryInfoUI::GetIsPinned() const
{
	return UIBase->GetIsPinned();
}