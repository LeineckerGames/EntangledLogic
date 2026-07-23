#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "SettingsTabControls.generated.h"

class UScrollBox;
class UInputKeySelector;

UCLASS()
class ENTANGLEDLOGIC_API USettingsTabControls : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // Controls Settings
    UPROPERTY(meta = (BindWidget))
    UScrollBox* ControlsContainer;

    // Buttons/InputKeySelectors
    // Camera
    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* RotateLeftKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* RotateRightKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* ZoomInKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* ZoomOutKeySelector;

    // Game
    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* RotateGateKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* DeleteModeKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* EditModeKeySelector;

    // Movement
    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* ForwardsKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* BackwardsKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* LeftKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* RightKeySelector;

    // Player
    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* LeftClickKeySelector;

    UPROPERTY(meta = (BindWidget))
    UInputKeySelector* RightClickKeySelector;

    // Sub-menus
    UPROPERTY(meta = (BindWidget))
    UWidget* RotateLeftHorizontalBox;

    UPROPERTY(meta = (BindWidget))
    UWidget* RotateRightHorizontalBox;
    
    // On change functions
    UFUNCTION()
    void OnRotateLeftKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnRotateRightKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnZoomInKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnZoomOutKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnRotateGateSelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnDeleteModeSelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnEditModeSelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnForwardsKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnBackwardsKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnLeftKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnRightKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnLeftClickKeySelected(FInputChord SelectedKey);

    UFUNCTION()
    void OnRightClickKeySelected(FInputChord SelectedKey);

    // Helper Functions
    UFUNCTION()
    FKey GetCurrentKeyForMapping(APlayerController* PC, FName MappingName);

    UFUNCTION()
    void ChangeCurrentKeyMapping(FInputChord SelectedKey, FName MappingName);

    UFUNCTION()
    void RefreshButtonUI();
};