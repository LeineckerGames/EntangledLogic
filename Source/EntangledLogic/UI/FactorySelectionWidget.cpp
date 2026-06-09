#include "FactorySelectionWidget.h"
#include "EntangledLogic/Core/Framework/ItemDataStructs.h"
#include "FactoryItemSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

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
					ItemSlot->SetFactoryActorClass(RowData->ItemAssetData.ActorComponent);
					InventoryWrapBox->AddChildToWrapBox(ItemSlot);
				}
			}
		}
	}
}