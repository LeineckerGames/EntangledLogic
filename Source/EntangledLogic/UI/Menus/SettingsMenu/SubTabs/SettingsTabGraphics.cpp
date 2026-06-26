#include "SettingsTabGraphics.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/EngineTypes.h"

void USettingsTabGraphics::NativeConstruct()
{
    Super::NativeConstruct();

    if (ResolutionComboBox) ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &USettingsTabGraphics::OnResolutionChanged);
    if (WindowModeComboBox) WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &USettingsTabGraphics::OnWindowModeChanged);
    if (VSyncToggle)
    {
        if (GEngine && GEngine->GetGameUserSettings())
        {
            VSyncToggle->SetIsChecked(GEngine->GetGameUserSettings()->IsVSyncEnabled());
        }

        VSyncToggle->OnCheckStateChanged.AddDynamic(this, &USettingsTabGraphics::OnVSyncChanged);
    }

    if (AreYouSureCanvas) AreYouSureCanvas->SetVisibility(ESlateVisibility::Collapsed);

    if (YesAYSButton) YesAYSButton->OnClicked.AddDynamic(this, &USettingsTabGraphics::ConfirmCurrentSettings);
    if (NoAYSButton) NoAYSButton->OnClicked.AddDynamic(this, &USettingsTabGraphics::RevertToLastConfirmedSettings);


    // Init
    // Null check the settings object before trying to pull current values from it
    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    if (!Settings) return;

    // Populates resolution dropdown with supported resolutions and sets the current selection to the current resolution
    if (ResolutionComboBox)
    {
        const TArray<FString>& ResArray = SupportedGameResolution();

        if (ResolutionComboBox->GetOptionCount() == 0)
        {
            for (int32 i = ResArray.Num() - 1; i >= 0; --i)
            {
                ResolutionComboBox->AddOption(ResArray[i]);
            }

            FIntPoint CurrentRes = Settings->GetScreenResolution();
            FString MatchString = FString::Printf(TEXT("%dx%d"), CurrentRes.X, CurrentRes.Y);
            ResolutionComboBox->SetSelectedOption(MatchString);
        }
    }

    // Populates window dropdown with supported window modes and sets the current selection to the current window mode
    if (WindowModeComboBox)
    {
        // Add window mode text options
        WindowModeComboBox->AddOption(TEXT("Fullscreen"));
        WindowModeComboBox->AddOption(TEXT("Windowed Fullscreen"));
        WindowModeComboBox->AddOption(TEXT("Windowed"));

        // Match the current window mode text to the current window mode
        EWindowMode::Type CurrentMode = Settings->GetFullscreenMode();

        switch (CurrentMode)
        {
        case EWindowMode::Fullscreen:
            WindowModeComboBox->SetSelectedIndex(0);
            break;
        case EWindowMode::WindowedFullscreen:
            WindowModeComboBox->SetSelectedIndex(1);
            break;
        case EWindowMode::Windowed:
            WindowModeComboBox->SetSelectedIndex(2);
            break;
        }
    }

    bIsInitialized = true;
}

// Resolution dropdown population function, queries the graphics driver for supported resolutions and returns them as an array of strings to be added to the resolution dropdown
TArray<FString> USettingsTabGraphics::SupportedGameResolution()
{
    TArray<FString> ResolutionStrings;
    FScreenResolutionArray Resolutions;

    // Queries the graphics driver for supported dimensions
    if (RHIGetAvailableResolutions(Resolutions, false))
    {
        for (const FScreenResolutionRHI& Res : Resolutions)
        {
            FString ResStr = FString::Printf(TEXT("%dx%d"), Res.Width, Res.Height);
            ResolutionStrings.AddUnique(ResStr);
        }
    }
    return ResolutionStrings;
}

// Resolution
void USettingsTabGraphics::OnResolutionChanged(FString Resolution, ESelectInfo::Type SelectionType)
{
    if (!bIsInitialized)
    {
        return;
    }

    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    if (!Settings) return;

    OldResolution = Settings->GetScreenResolution();

    // Parse the resolution string into width and height
    FString WidthString, HeightString;
    int32 Width, Height;

    Resolution.Split(TEXT("x"), &WidthString, &HeightString);
    Width = FCString::Atoi(*WidthString);
    Height = FCString::Atoi(*HeightString);

    // Set the new resolution
    Settings->SetScreenResolution(FIntPoint(Width, Height));
    Settings->ApplySettings(false);

    // Prompt the user to confirm or revert the new settings
    if (AreYouSureCanvas) AreYouSureCanvas->SetVisibility(ESlateVisibility::Visible);

    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        PC->bShowMouseCursor = true;

        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(InputMode);
    }

    StartRevertTimer();
}

// Window Mode
void USettingsTabGraphics::OnWindowModeChanged(FString WindowMode, ESelectInfo::Type SelectionType)
{
    // If called during initialization, don't apply settings or show confirmation prompt, as the dropdown is being populated and set to the current setting in LoadCurrentSettings()
    if (SelectionType == ESelectInfo::Direct) return;

    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    if (!Settings) return;

    OldWindowMode = Settings->GetFullscreenMode();

    int32 SelectedIndex = WindowModeComboBox->GetSelectedIndex();
    EWindowMode::Type NewMode = static_cast<EWindowMode::Type>(SelectedIndex);

    // Set the new window mode
    Settings->SetFullscreenMode(NewMode);
    Settings->ApplySettings(false);

    // Prompt the user to confirm or revert the new settings
    if (AreYouSureCanvas) AreYouSureCanvas->SetVisibility(ESlateVisibility::Visible);
    StartRevertTimer();
}

// VSync
void USettingsTabGraphics::OnVSyncChanged(bool bIsChecked)
{
    if (!bIsInitialized)
    {
        return;
    }

    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    if (!Settings) return;

    // Set the new VSync setting
    Settings->SetVSyncEnabled(bIsChecked);
    Settings->ApplySettings(false);
}

// Are You Sure Timer
void USettingsTabGraphics::StartRevertTimer()
{
    CountdownSecondsRemaining = 15;

    if (CountdownText)
    {
        FText FormatTemplate = FText::FromString(TEXT("Time remaining: {0} seconds"));
        FText FormattedDisplay = FText::Format(FormatTemplate, FText::AsNumber(CountdownSecondsRemaining));
        CountdownText->SetText(FormattedDisplay);
    }
    UE_LOG(LogTemp, Warning, TEXT("Starting loop"));
    // Start a looping timer running every 1 second
    GetWorld()->GetTimerManager().SetTimer(
        CountdownTimerHandle,
        this,
        &USettingsTabGraphics::UpdateCountdown,
        1.0f,
        true
    );
    UE_LOG(LogTemp, Warning, TEXT("Loop started"));
}

// Are You Sure Timer Update
void USettingsTabGraphics::UpdateCountdown()
{
    CountdownSecondsRemaining--;

    UE_LOG(LogTemp, Warning, TEXT("Timer Ticking! Seconds Remaining: %d"), CountdownSecondsRemaining);

    if (CountdownText)
    {
        FText FormatTemplate = FText::FromString(TEXT("Time remaining: {0} seconds"));
        FText FormattedDisplay = FText::Format(FormatTemplate, FText::AsNumber(CountdownSecondsRemaining));
        CountdownText->SetText(FormattedDisplay);
    }

    // Timer ran out! Revert automatically
    if (CountdownSecondsRemaining <= 0)
    {
        RevertToLastConfirmedSettings();
    }
}

// Confirm the new settings if the user clicks "Yes" within the time limit
void USettingsTabGraphics::ConfirmCurrentSettings()
{
    GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    if (!Settings) return;

    // Confirm the settings
    Settings->ConfirmVideoMode();
    Settings->SaveSettings();

    // Hide the confirmation prompt
    if (AreYouSureCanvas) AreYouSureCanvas->SetVisibility(ESlateVisibility::Collapsed);

    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }
}

// Revert to the last confirmed settings if the user clicks "No" or fails to confirm within the time limit
void USettingsTabGraphics::RevertToLastConfirmedSettings()
{
    GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

    UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
    if (!Settings) return;

    // Revert to the last confirmed settings
    Settings->RevertVideoMode();
    Settings->ApplySettings(false);

    // Hide the confirmation prompt
    if (AreYouSureCanvas) AreYouSureCanvas->SetVisibility(ESlateVisibility::Collapsed);

    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }

    // Refresh UI using OldResolution and OldWindowMode
}