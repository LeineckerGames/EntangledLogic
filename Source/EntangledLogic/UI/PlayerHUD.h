#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UFactorySelectionWidget;
class UPlayerControlsUI;
class UPauseMenuWidget;
class USettingsMenuWidget;

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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USettingsMenuWidget> SettingsMenuWidgetClass;

	APlayerHUD();

	void RepopulateFactorySelectionWidget();

	void UpdatePlayerControlsUI();

	void UpdatePlayerControlsUI(EPlacementMode PlacementMode);

	void TogglePauseMenu();

	void OpenPauseMenu();

	void ClosePauseMenu();

	void OpenSettingsMenu();

	void CloseSettingsMenu();

	void QuitToMainMenu();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UFactorySelectionWidget* FactorySelectionWidget;

	UPROPERTY()
	UPlayerControlsUI* PlayerControlsUIWidget;

	UPROPERTY()
	UPauseMenuWidget* PauseMenuWidget;

	UPROPERTY()
	USettingsMenuWidget* SettingsMenuWidget;
	
};
