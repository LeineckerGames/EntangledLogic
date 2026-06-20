#include "ControlBase.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"


void UControlBase::AddIconToUI(UTexture2D* IconToAdd)
{
	if (UWorld* World = GetWorld())
	{
		UImage* Image = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
		if (Image)
		{
			Image->SetBrushFromTexture(IconToAdd);
			UHorizontalBoxSlot* HorizontalSlot = Cast<UHorizontalBoxSlot>(IconBox->AddChild(Image));
			if (HorizontalSlot)
			{
				FSlateChildSize SizeRules;
				SizeRules.SizeRule = ESlateSizeRule::Automatic;
				SizeRules.Value = 100.0f;
				HorizontalSlot->SetSize(SizeRules);
			}
			
		}
	}
}

void UControlBase::SetControlText(FString TextToSet)
{
	if (ControlText)
	{
		ControlText->SetText(FText::FromString(TextToSet));
	}
}

void UControlBase::SetControlText(FText TextToSet)
{
	if (ControlText)
	{
		ControlText->SetText(TextToSet);
	}
}