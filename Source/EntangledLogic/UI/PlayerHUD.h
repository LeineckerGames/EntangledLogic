#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UFactorySelectionWidget;
class UPlayerControlsUI;

enum class EPlacementMode : uint8;
UCLASS()
class ENTANGLEDLOGIC_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UFactorySelectionWidget> FactorySelectionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UPlayerControlsUI> PlayerControlsUIClass;

	APlayerHUD();

	void RepopulateFactorySelectionWidget();

	void UpdatePlayerControlsUI(EPlacementMode PlacementMode);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UFactorySelectionWidget* FactorySelectionWidget;

	UPROPERTY()
	UPlayerControlsUI* PlayerControlsUIWidget;
	
};
