#include "FactorySelectionWidget.h"
#include "EntangledLogic/Core/Framework/ItemDataStructs.h"
#include "FactoryItemSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h" 

void UFactorySelectionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PopulateInventory();
}

void UFactorySelectionWidget::PopulateInventory()
{
	if (InventoryItemSlotClass && FactoriesTable)
	{
		InventoryWrapBox->ClearChildren();

		TArray<FName> FactoriesNames = FactoriesTable->GetRowNames();
		FString ContextString;

		// Iterate through Data Table and insert Inventory slots into the Wrap Box.
		for (auto& name : FactoriesNames)
		{
			FItemData* RowData = FactoriesTable->FindRow<FItemData>(name, ContextString);
			if (RowData)
			{
				if (RowData->ItemAssetData.Icon) {
					UFactoryItemSlot* ItemSlot = CreateWidget<UFactoryItemSlot>(this, InventoryItemSlotClass);
					ItemSlot->SetFactoryIcon(RowData->ItemAssetData.Icon);
					ItemSlot->SetFactoryName(RowData->ItemTextData.Name.ToString());
					ItemSlot->SetFactoryDescription(RowData->ItemTextData.Description.ToString());
					ItemSlot->SetFactoryActorClass(RowData->ItemAssetData.ActorComponent);
					ItemSlot->FactorySelectionWidget = this;
					InventoryWrapBox->AddChildToWrapBox(ItemSlot);
				}
			}
		}
	}
}

void UFactorySelectionWidget::SetNameAndDescriptionText(FString FactoryName, FString FactoryDescription)
{
	FString CombinedNameAndDesc = FactoryName + " - " + FactoryDescription;
	//UE_LOG(LogTemp, Display, TEXT("Update Name to : %s"), *CombinedNameAndDesc);
	FText TextNameAndDesc = FText::FromString(CombinedNameAndDesc);
	FactoryTextBox->SetText(TextNameAndDesc);
}

void UFactorySelectionWidget::ClearNameAndDescriptionText()
{
	FText DefaultText = FText::FromString("Factory List:");
	FactoryTextBox->SetText(DefaultText);
}