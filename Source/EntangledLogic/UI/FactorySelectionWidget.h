

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
	class UTextBlock* FactoryTextBox;

	UPROPERTY(meta = (BindWidget))
	class UWrapBox* InventoryWrapBox;

	UPROPERTY(EditAnywhere)
	class UDataTable* FactoriesTable;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFactoryItemSlot> InventoryItemSlotClass;

	void SetNameAndDescriptionText(FString FactoryName, FString FactoryDescription);

	void ClearNameAndDescriptionText();

	void PopulateInventory();

protected:
	virtual void NativeOnInitialized() override;

};
