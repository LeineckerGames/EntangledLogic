#include "ControlBase.h"
#include "Engine/Texture2D.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
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
			IconBox->AddChild(Image);
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