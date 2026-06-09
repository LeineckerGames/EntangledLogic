

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FactoryItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class ENTANGLEDLOGIC_API UFactoryItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetFactoryIcon(UTexture2D* FactoryIconToSet);
	void SetFactoryName(FString FactoryNameToSet);
	void SetFactoryDescription(FString FactoryDescriptionToSet);
	void SetFactoryActorClass(TSubclassOf<AActor> FactoryActorToSet);
	class UFactorySelectionWidget* FactorySelectionWidget;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	// UI Elements
	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	class UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	class UImage* ItemIcon;

	// Factory Info
	FString FactoryName;

	FString FactoryDescription;

	class UTexture2D* FactoryIcon;

	TSubclassOf<AActor> FactoryActor;
};
