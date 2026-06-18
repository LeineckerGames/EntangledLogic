#include "PlayerHUD.h"
#include "FactorySelectionWidget.h"
#include "PlayerControlsUI.h"

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

	// If Widget Class is set in editor Create it and add to screen
	if (FactorySelectionWidgetClass)
	{
		PlayerControlsUIWidget = CreateWidget<UPlayerControlsUI>(GetWorld(), PlayerControlsUIClass);
		PlayerControlsUIWidget->AddToViewport();
		//PlayerControlsUIWidget->UpdatePlayerControlsUI();
	}

}

void APlayerHUD::UpdatePlayerControlsUI()
{
	if (PlayerControlsUIWidget)
	{
		PlayerControlsUIWidget->UpdatePlayerControlsUI();
	}
}

void APlayerHUD::RepopulateFactorySelectionWidget()
{
	if (FactorySelectionWidget)
	{
		FactorySelectionWidget->PopulateInventory();
	}
}