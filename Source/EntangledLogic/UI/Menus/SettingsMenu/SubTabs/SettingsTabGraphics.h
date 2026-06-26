#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsTabGraphics.generated.h"

class UButton;
class UCheckBox;
class UComboBoxString;
class UTextBlock;

UCLASS()
class ENTANGLEDLOGIC_API USettingsTabGraphics : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    bool bIsInitialized = false;

    // Graphics Settings
    UPROPERTY(meta = (BindWidget))
    UComboBoxString* ResolutionComboBox;

    UPROPERTY(meta = (BindWidget))
    UComboBoxString* WindowModeComboBox;

    UPROPERTY(meta = (BindWidget))
    UCheckBox* VSyncToggle;

    // Are You Sure Confirmation Prompt
    UPROPERTY(meta = (BindWidget))
    UWidget* AreYouSureCanvas;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* CountdownText;

    UPROPERTY(meta = (BindWidget))
    UButton* YesAYSButton;

    UPROPERTY(meta = (BindWidget))
    UButton* NoAYSButton;

    FIntPoint OldResolution;
    EWindowMode::Type OldWindowMode;

    FTimerHandle CountdownTimerHandle;
    int32 CountdownSecondsRemaining;

    // Graphics functions
    UFUNCTION() static TArray<FString> SupportedGameResolution();
    UFUNCTION() void OnResolutionChanged(FString Resolution, ESelectInfo::Type SelectionType);
    UFUNCTION() void OnWindowModeChanged(FString Resolution, ESelectInfo::Type SelectionType);
    UFUNCTION() void OnVSyncChanged(bool bIsChecked);

    // Are You Sure functions
    UFUNCTION() void StartRevertTimer();
    UFUNCTION() void UpdateCountdown();
    UFUNCTION() void ConfirmCurrentSettings();
    UFUNCTION() void RevertToLastConfirmedSettings();
};
