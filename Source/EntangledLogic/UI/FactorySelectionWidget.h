

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Engine/DataTable.h"
#include "FactorySelectionWidget.generated.h"

class UFactoryItemSlot;

UCLASS()
class ENTANGLEDLOGIC_API UFactorySelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* InventoryWrapBox;

	UPROPERTY(EditAnywhere)
	class UDataTable* FactoriesTable;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFactoryItemSlot> InventoryItemSlotClass;

protected:
	virtual void NativeOnInitialized() override;

	void PopulateInventory();

};
