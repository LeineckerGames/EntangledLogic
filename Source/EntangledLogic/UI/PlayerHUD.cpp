
#include "PlayerHUD.h"
#include "FactorySelectionWidget.h"

APlayerHUD::APlayerHUD()
{

}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	// If Widget Class is set in editor Create it and add to screen
	if (FactorySelectionWidgetClass)
	{
		FactorySelectionWidget = CreateWidget<UFactorySelectionWidget>(GetWorld(), FactorySelectionWidgetClass);
		FactorySelectionWidget->AddToViewport();
	}

}