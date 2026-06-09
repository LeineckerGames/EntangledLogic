#include "FactoryItemSlot.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "FactorySelectionWidget.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"

void UFactoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UFactoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	ItemIcon->SetBrushFromTexture(FactoryIcon);
}

FReply UFactoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

	UE_LOG(LogTemp, Display, TEXT("Clicked on %s"), *FactoryName);
	
	// Get Grid Placement Subsystem Here instead
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();

	// Set the held factory to item selection.
	

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UFactoryItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (FactorySelectionWidget)
	{
		FactorySelectionWidget->SetNameAndDescriptionText(FactoryName, FactoryDescription);
	}
}

void UFactoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (FactorySelectionWidget)
	{
		FactorySelectionWidget->ClearNameAndDescriptionText();
	}
}

// Setters

void UFactoryItemSlot::SetFactoryIcon(UTexture2D* FactoryIconToSet)
{
	FactoryIcon = FactoryIconToSet;
}

void UFactoryItemSlot::SetFactoryName(FString FactoryNameToSet)
{
	FactoryName = FactoryNameToSet;
}

void UFactoryItemSlot::SetFactoryDescription(FString FactoryDescriptionToSet)
{
	FactoryDescription = FactoryDescriptionToSet;
}

void UFactoryItemSlot::SetFactoryActorClass(TSubclassOf<AActor> FactoryActorToSet)
{
	FactoryActor = FactoryActorToSet;
	//UE_LOG(LogTemp, Display, TEXT("Inventory Item Slot: Factory Actor Set to %s"))
}