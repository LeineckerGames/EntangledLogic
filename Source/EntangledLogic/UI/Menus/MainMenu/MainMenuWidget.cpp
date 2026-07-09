#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EntangledLogic/Core/Subsystems/SavingLoadingSubsystem.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/SettingsMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button references to blueprint buttons and assign on click events
    if (AnywhereClickButton) AnywhereClickButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnAnywhereClicked);

    if (PlayButton) PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
    if (LoadScreenBackButton) LoadScreenBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadScreenBackClicked);
    if (LoadSlot1Button) LoadSlot1Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadSlot1ButtonClicked);
    if (LoadSlot2Button) LoadSlot2Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadSlot2ButtonClicked);
    if (LoadSlot3Button) LoadSlot3Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadSlot3ButtonClicked);
    if (LoadSlot4Button) LoadSlot4Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadSlot4ButtonClicked);

    if (DeleteMenuButton) DeleteMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeleteMenuButtonClicked);
    if (CloseDeleteMenuButton) CloseDeleteMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCloseDeleteMenuButtonClicked);
    if (DeleteSaveSlot1Button) DeleteSaveSlot1Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeleteSaveSlot1Clicked);
    if (DeleteSaveSlot2Button) DeleteSaveSlot2Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeleteSaveSlot2Clicked);
    if (DeleteSaveSlot3Button) DeleteSaveSlot3Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeleteSaveSlot3Clicked);
    if (DeleteSaveSlot4Button) DeleteSaveSlot4Button->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeleteSaveSlot4Clicked);
    if (DeleteYesButton) DeleteYesButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeleteYesButtonClicked);
    if (DeleteNoButton) DeleteNoButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnDeleteNoButtonClicked);

    if (SettingsButton)  SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);

    if (CreditsButton)   CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreditsClicked);
    if (CloseCreditsButton)  CloseCreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCloseCreditsClicked);

    if (QuitGameButton)  QuitGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitGameClicked);

    // Ensure appropriate sub-menus are completely hidden on boot
    if (TitleScreenGroup) TitleScreenGroup->SetVisibility(ESlateVisibility::Visible);
    if (MainMenuOptionsGroup) MainMenuOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (CreditsPanel)  CreditsPanel->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadOptionsGroup)  LoadOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteOptionsGroup)  DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS)  DeleteAYS->SetVisibility(ESlateVisibility::Collapsed);
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
	// Refresh the load slot text blocks with the latest save data
    RefreshLoadSlotButtons();

	// Hide the main menu options and show the load options
    if (MainMenuOptionsGroup) MainMenuOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Return from play button
void UMainMenuWidget::OnLoadScreenBackClicked()
{
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (MainMenuOptionsGroup) MainMenuOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Load from slot 1
void UMainMenuWidget::OnLoadSlot1ButtonClicked()
{
    TriggerLoad(1);
    UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

// Load from slot 2
void UMainMenuWidget::OnLoadSlot2ButtonClicked()
{
    TriggerLoad(2);
    UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

// Load from slot 3
void UMainMenuWidget::OnLoadSlot3ButtonClicked()
{
    TriggerLoad(3);
    UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

// Load from slot 4
void UMainMenuWidget::OnLoadSlot4ButtonClicked()
{
    TriggerLoad(4);
    UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

// Return from play button
void UMainMenuWidget::RefreshLoadSlotButtons()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            FSlotMetadata Slot1Data = SaveSystem->GetMetadataForSlot(1);
            if (LoadSlot1TextBlock)
            {
                FString DisplayString = Slot1Data.bIsSlotEmpty ?
                    TEXT("Slot 1: Empty") :
                    FString::Printf(TEXT("%s | (%s)"), *Slot1Data.PlayerName, *Slot1Data.DateSaved);

                LoadSlot1TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot1TextBlock->SetText(FText::FromString(DisplayString));
            }

            FSlotMetadata Slot2Data = SaveSystem->GetMetadataForSlot(2);
            if (LoadSlot2TextBlock)
            {
                FString DisplayString = Slot2Data.bIsSlotEmpty ?
                    TEXT("Slot 2: Empty") :
                    FString::Printf(TEXT("%s | (%s)"), *Slot2Data.PlayerName, *Slot2Data.DateSaved);

                LoadSlot2TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot2TextBlock->SetText(FText::FromString(DisplayString));
            }

            FSlotMetadata Slot3Data = SaveSystem->GetMetadataForSlot(3);
            if (LoadSlot3TextBlock)
            {
                FString DisplayString = Slot3Data.bIsSlotEmpty ?
                    TEXT("Slot 3: Empty") :
                    FString::Printf(TEXT("%s | (%s)"), *Slot3Data.PlayerName, *Slot3Data.DateSaved);

                LoadSlot3TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot3TextBlock->SetText(FText::FromString(DisplayString));
            }

            FSlotMetadata Slot4Data = SaveSystem->GetMetadataForSlot(4);
            if (LoadSlot4TextBlock)
            {
                FString DisplayString = Slot4Data.bIsSlotEmpty ?
                    TEXT("Slot 4: Empty") :
                    FString::Printf(TEXT("%s | (%s)"), *Slot4Data.PlayerName, *Slot4Data.DateSaved);

                LoadSlot4TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot4TextBlock->SetText(FText::FromString(DisplayString));
            }
        }
    }
}

// Opens the delete menu
void UMainMenuWidget::OnDeleteMenuButtonClicked()
{
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Closes the delete menu and returns to the pause menu
void UMainMenuWidget::OnCloseDeleteMenuButtonClicked()
{
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UMainMenuWidget::OnDeleteSaveSlot1Clicked()
{
    PendingSaveSlot = 1;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UMainMenuWidget::OnDeleteSaveSlot2Clicked()
{
    PendingSaveSlot = 2;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UMainMenuWidget::OnDeleteSaveSlot3Clicked()
{
    PendingSaveSlot = 3;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UMainMenuWidget::OnDeleteSaveSlot4Clicked()
{
    PendingSaveSlot = 4;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Deletes the save file in the selected slot and returns to the save menu
void UMainMenuWidget::OnDeleteYesButtonClicked()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            SaveSystem->DeleteSaveFile(PendingSaveSlot);
            RefreshLoadSlotButtons();
        }
    }

    PendingSaveSlot = -1;
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadOptionsGroup) LoadOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Return to the delete menu without deleting
void UMainMenuWidget::OnDeleteNoButtonClicked()
{
    PendingSaveSlot = -1;
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Settings button shout to MainMenuPlayerController/MainMenuHUD
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

// Quit button shout to MainMenuPlayerController/MainMenuHUD
void UMainMenuWidget::OnQuitGameClicked()
{
    OnQuitButtonClicked.Broadcast();
}

// Load helper function
USavingLoadingSubsystem* UMainMenuWidget::GetSaveSystem() const
{
    if (UGameInstance* GI = GetGameInstance())
    {
        return GI->GetSubsystem<USavingLoadingSubsystem>();
    }
    return nullptr;
}

// Trigger load
void UMainMenuWidget::TriggerLoad(int32 SlotIndex)
{
    if (USavingLoadingSubsystem* SaveSystem = GetSaveSystem())
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);

        SetVisibility(ESlateVisibility::Collapsed);

        SaveSystem->RequestLoad(SlotIndex);
    }
}