#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "SaveLoad/SaveGameData.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/SettingsMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button references to blueprint buttons and assign on click events
    if (AnywhereClickButton) AnywhereClickButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnAnywhereClicked);

    if (PlayButton) PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
    if (LoadScreenBackButton) LoadScreenBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadScreenBackClicked);

    if (SettingsButton)  SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);

    if (CreditsButton)   CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreditsClicked);
    if (CloseCreditsButton)  CloseCreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCloseCreditsClicked);

    if (QuitGameButton)  QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGameClicked);

    // Ensure appropriate sub-menus are completely hidden on boot
    if (TitleScreenGroup) TitleScreenGroup->SetVisibility(ESlateVisibility::Visible);
    if (MainMenuOptionsGroup) MainMenuOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (CreditsPanel)  CreditsPanel->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadOptionsGroup)  LoadOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
}

// Big invisible button
void UMainMenuWidget::OnAnywhereClicked()
{
    // Kill screen-wide interceptor button
    if (AnywhereClickButton) AnywhereClickButton->SetVisibility(ESlateVisibility::Collapsed);

    // Hide the "Press Anywhere to Start / Logo"
    if (TitleScreenGroup) TitleScreenGroup->SetVisibility(ESlateVisibility::Collapsed);

    // Reveal the actual menu selection
    if (MainMenuOptionsGroup)MainMenuOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Play button
void UMainMenuWidget::OnPlayClicked()
{
    // Hide the actual menu selection
    if (MainMenuOptionsGroup) MainMenuOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Visible);

	// TODO: Load save slots from disk and display them in the load options group
    //LoadSave();
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

// Return from play button
void UMainMenuWidget::OnLoadScreenBackClicked()
{
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (MainMenuOptionsGroup) MainMenuOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Settings button shout to MainMenuPlayerController
void UMainMenuWidget::OnSettingsClicked()
{
    OnSettingsButtonClicked.Broadcast();
}

// Credits button
void UMainMenuWidget::OnCreditsClicked()
{
    if (CreditsPanel) CreditsPanel->SetVisibility(ESlateVisibility::Visible);
}

// Return from credits button
void UMainMenuWidget::OnCloseCreditsClicked()
{
    if (CreditsPanel) CreditsPanel->SetVisibility(ESlateVisibility::Collapsed);
}

// Quit button shout to MainMenuPlayerController
void UMainMenuWidget::OnQuitGameClicked()
{
    OnQuitButtonClicked.Broadcast();
}

/*
// Load player save slots from disk and return them to the menu for display
TArray<FSaveSlotInfo> UMainMenuWidget::GetAvailableSaveSlots() const
{

}

// On click of a save slot, determine if it is filled or empty and either load or create a new save file accordingly
void UMainMenuWidget::ProcessSlotSelection(const FSaveSlotInfo& TargetSlot, const FString& CustomNewName)
{
    UGameInstance* GameInst = GetGameInstance();
    if (!GameInst) return;

    UGameSaveSubsystem* SaveSys = GameInst->GetSubsystem<UGameSaveSubsystem>();
    if (!SaveSys) return;

    if (TargetSlot.bIsFilled)
    {
        // 1. FILLED SLOT: Safely cast down to the Load Contract interface and boot it up
        if (ILoadExecutor* LoadEngine = Cast<ILoadExecutor>(SaveSys))
        {
            LoadEngine->ExecuteLoad(TargetSlot.SlotName);
        }
    }
    else
    {
        // 2. UNFILLED SLOT: Save initialization block first (sets up the file)
        if (ISaveExecutor* SaveEngine = Cast<ISaveExecutor>(SaveSys))
        {
            // Explicitly pass the slot name and the player's newly typed custom text entry.
            // Your ExecuteSave implementation will assign CustomNewName to the save file.
            SaveEngine->ExecuteSave(TargetSlot.SlotName);
        }

        // 3. Immediately follow up by loading that freshly generated file
        if (ILoadExecutor* LoadEngine = Cast<ILoadExecutor>(SaveSys))
        {
            LoadEngine->ExecuteLoad(TargetSlot.SlotName);
        }
    }
}

// Copy of above function
void UMainMenuWidget::LoadSave()
{
    if (UGameInstance* GameInst = GetGameInstance())
    {
        if (UGameSaveSubsystem* SaveSys = GameInst->GetSubsystem<UGameSaveSubsystem>())
        {
            // Call our dynamic disk scanner function
            return SaveSys->GetFixedSaveSlots();
        }
    }
    return TArray<FSaveSlotInfo>();
}*/
