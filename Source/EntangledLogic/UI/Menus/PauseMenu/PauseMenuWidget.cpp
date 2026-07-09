// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"
#include "EntangledLogic/Core/Subsystems/SavingLoadingSubsystem.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button references to blueprint buttons and assign on click events
    if (ResumeButton) ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);

    if (SettingsButton) SettingsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSettingsClicked);

    if (SaveMenuButton) SaveMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSaveMenuClicked);
    if (CloseSaveMenuButton) CloseSaveMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCloseSaveMenuClicked);
    if (SaveSlot1Button) SaveSlot1Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSaveSlot1Clicked);
    if (SaveSlot2Button) SaveSlot2Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSaveSlot2Clicked);
    if (SaveSlot3Button) SaveSlot3Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSaveSlot3Clicked);
    if (SaveSlot4Button) SaveSlot4Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSaveSlot4Clicked);
    if (OverwriteYesButton) OverwriteYesButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOverwriteYesButtonClicked);
    if (OverwriteNoButton) OverwriteNoButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOverwriteNoButtonClicked);

    if (DeleteMenuButton) DeleteMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDeleteMenuButtonClicked);
    if (CloseDeleteMenuButton) CloseDeleteMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCloseDeleteMenuButtonClicked);
    if (DeleteSaveSlot1Button) DeleteSaveSlot1Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDeleteSaveSlot1Clicked);
    if (DeleteSaveSlot2Button) DeleteSaveSlot2Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDeleteSaveSlot2Clicked);
    if (DeleteSaveSlot3Button) DeleteSaveSlot3Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDeleteSaveSlot3Clicked);
    if (DeleteSaveSlot4Button) DeleteSaveSlot4Button->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDeleteSaveSlot4Clicked);
    if (DeleteYesButton) DeleteYesButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDeleteYesButtonClicked);
    if (DeleteNoButton) DeleteNoButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDeleteNoButtonClicked);

    if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
    if (CloseQuitMenuButton) CloseQuitMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCloseQuitClicked);
    if (QuitToMainMenuButton) QuitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
    // Commented out because quitting to desktop is kinda sudden. Left here incase a different team wants it.
    // if (QuitToDesktopButton) QuitToDesktopButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDesktopClicked);

    // Ensure correct sub-menus are completely hidden on boot
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Visible);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (QuitOptionsGroup) QuitOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
}

// Hide the pause menu and return to game
void UPauseMenuWidget::OnResumeClicked()
{
    OnResumeButtonClicked.Broadcast();
}

// Hide the pause menu and show the settings
void UPauseMenuWidget::OnSettingsClicked()
{
    OnSettingsButtonClicked.Broadcast();
}

// Hide the pause menu and show the save menu
void UPauseMenuWidget::OnSaveMenuClicked()
{
    RefreshSaveSlotButtons();
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Hide the save menu and show the pause menu
void UPauseMenuWidget::OnCloseSaveMenuClicked()
{
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Save data into SaveSlot1
void UPauseMenuWidget::OnSaveSlot1Clicked()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            FSlotMetadata SlotData = SaveSystem->GetMetadataForSlot(1);
            if (SlotData.bIsSlotEmpty)
            {
                SaveSystem->RequestSave(1, SlotData.PlayerName);
				RefreshSaveSlotButtons();
            }
            else
            {
                PendingSaveSlot = 1;
                if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
                if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}

// Save data into SaveSlot2
void UPauseMenuWidget::OnSaveSlot2Clicked()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            FSlotMetadata SlotData = SaveSystem->GetMetadataForSlot(2);
            if (SlotData.bIsSlotEmpty)
            {
                SaveSystem->RequestSave(2, SlotData.PlayerName);
                RefreshSaveSlotButtons();
            }
            else
            {
                PendingSaveSlot = 2;
                if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
                if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}

// Save data into SaveSlot3
void UPauseMenuWidget::OnSaveSlot3Clicked()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            FSlotMetadata SlotData = SaveSystem->GetMetadataForSlot(3);
            if (SlotData.bIsSlotEmpty)
            {
                SaveSystem->RequestSave(3, SlotData.PlayerName);
                RefreshSaveSlotButtons();
            }
            else
            {
                PendingSaveSlot = 3;
                if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
                if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}

// Save data into SaveSlot4
void UPauseMenuWidget::OnSaveSlot4Clicked()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            FSlotMetadata SlotData = SaveSystem->GetMetadataForSlot(4);
            if (SlotData.bIsSlotEmpty)
            {
                SaveSystem->RequestSave(4, SlotData.PlayerName);
                RefreshSaveSlotButtons();
            }
            else
            {
                PendingSaveSlot = 4;
                if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
                if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Visible);
            }
        }
    }
}

// Refreshes the save slot buttons with the latest metadata from the SavingLoadingSubsystem
void UPauseMenuWidget::RefreshSaveSlotButtons()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            FSlotMetadata Slot1Data = SaveSystem->GetMetadataForSlot(1);
            if (SaveSlot1TextBlock)
            {
                FString DisplayString = Slot1Data.bIsSlotEmpty ?
                    TEXT("Slot 1: Empty") :
                    FString::Printf(TEXT("%s | (%s)"), *Slot1Data.PlayerName, *Slot1Data.DateSaved);

                SaveSlot1TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot1TextBlock->SetText(FText::FromString(DisplayString));
            }

            FSlotMetadata Slot2Data = SaveSystem->GetMetadataForSlot(2);
            if (SaveSlot2TextBlock)
            {
                FString DisplayString = Slot2Data.bIsSlotEmpty ?
                    FString(TEXT("Slot 2: Empty")) :
                    FString::Printf(TEXT("%s | (%s)"), *Slot2Data.PlayerName, *Slot2Data.DateSaved);
                SaveSlot2TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot2TextBlock->SetText(FText::FromString(DisplayString));
            }

            FSlotMetadata Slot3Data = SaveSystem->GetMetadataForSlot(3);
            if (SaveSlot3TextBlock)
            {
                FString DisplayString = Slot3Data.bIsSlotEmpty ?
                    TEXT("Slot 3: Empty") :
                    FString::Printf(TEXT("%s | (%s)"), *Slot3Data.PlayerName, *Slot3Data.DateSaved);

                SaveSlot3TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot3TextBlock->SetText(FText::FromString(DisplayString));
            }

            FSlotMetadata Slot4Data = SaveSystem->GetMetadataForSlot(4);
            if (SaveSlot4TextBlock)
            {
                FString DisplayString = Slot4Data.bIsSlotEmpty ?
                    TEXT("Slot 4: Empty") :
                    FString::Printf(TEXT("%s | (%s)"), *Slot4Data.PlayerName, *Slot4Data.DateSaved);

                SaveSlot4TextBlock->SetText(FText::FromString(DisplayString));
                DeleteSaveSlot4TextBlock->SetText(FText::FromString(DisplayString));
            }
        }
    }
}

// Execute the save operation and return to the pause menu
void UPauseMenuWidget::OnOverwriteYesButtonClicked()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            FString UserTypedName = TEXT("");
            SaveSystem->RequestSave(PendingSaveSlot, UserTypedName);
			RefreshSaveSlotButtons();
        }
    }

    PendingSaveSlot = -1;
    if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Return to the pause menu without saving
void UPauseMenuWidget::OnOverwriteNoButtonClicked()
{
    PendingSaveSlot = -1;
    if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Opens the delete menu
void UPauseMenuWidget::OnDeleteMenuButtonClicked()
{
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Closes the delete menu and returns to the pause menu
void UPauseMenuWidget::OnCloseDeleteMenuButtonClicked()
{
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UPauseMenuWidget::OnDeleteSaveSlot1Clicked()
{
    PendingSaveSlot = 1;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UPauseMenuWidget::OnDeleteSaveSlot2Clicked()
{
    PendingSaveSlot = 2;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UPauseMenuWidget::OnDeleteSaveSlot3Clicked()
{
    PendingSaveSlot = 3;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Opens the Delete Are You Sure menu for the selected save slot
void UPauseMenuWidget::OnDeleteSaveSlot4Clicked()
{
    PendingSaveSlot = 4;
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Visible);
}

// Deletes the save file in the selected slot and returns to the save menu
void UPauseMenuWidget::OnDeleteYesButtonClicked()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (USavingLoadingSubsystem* SaveSystem = GI->GetSubsystem<USavingLoadingSubsystem>())
        {
            SaveSystem->DeleteSaveFile(PendingSaveSlot);
            RefreshSaveSlotButtons();
        }
    }

    PendingSaveSlot = -1;
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Return to the delete menu without deleting
void UPauseMenuWidget::OnDeleteNoButtonClicked()
{
    PendingSaveSlot = -1;
    if (DeleteAYS) DeleteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (DeleteOptionsGroup) DeleteOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Hide the pause menu and show the quit options
void UPauseMenuWidget::OnQuitClicked()
{
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (QuitOptionsGroup) QuitOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Hide the quit options and show the pause menu
void UPauseMenuWidget::OnCloseQuitClicked()
{
    if (QuitOptionsGroup) QuitOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Fetches the player controller to send it to the main menu
void UPauseMenuWidget::OnMainMenuClicked()
{
    OnQuitToMainMenuButtonClicked.Broadcast();
}

// Fetches the player controller to safely shut down the game process
// Commented out because quitting to desktop is kinda sudden. Left here incase a different team wants it.
// All you have to do is delare a dynamic multicast delegate in the header, then bind it to a function in
// the player controller that calls UKismetSystemLibrary::QuitGame() with the player controller as the first argument.
/*
void UPauseMenuWidget::OnDesktopClicked()
{
    OnQuitToDesktopButtonClicked.Broadcast();
}
*/