// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/Widget.h"
//#include "SaveLoad/GameSaveSubsystem.h"

void UPauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button references to blueprint buttons and assign on click events
    if (ResumeButton) ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnResumeClicked);

    if (SettingsButton) SettingsButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSettingsClicked);

    if (SaveMenuButton) SaveMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnSaveMenuClicked);
    if (CloseSaveMenuButton) CloseSaveMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCloseSaveMenuClicked);
    if (OverwriteYesButton) OverwriteYesButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOverwriteYesButtonClicked);
    if (OverwriteNoButton) OverwriteNoButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnOverwriteNoButtonClicked);

    if (QuitButton) QuitButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnQuitClicked);
    if (CloseQuitMenuButton) CloseQuitMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCloseQuitClicked);
    if (QuitToMainMenuButton) QuitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnMainMenuClicked);
    // Commented out because quitting to desktop is kinda sudden. Left here incase a different team wants it.
    // if (QuitToDesktopButton) QuitToDesktopButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnDesktopClicked);

    // Ensure sub-menus are completely hidden on boot
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Visible);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (QuitOptionsGroup) QuitOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Collapsed);
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
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Hide the save menu and show the pause menu
void UPauseMenuWidget::OnCloseSaveMenuClicked()
{
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Collapsed);
    if (PauseOptionsGroup) PauseOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Execute the save operation and return to the pause menu
void UPauseMenuWidget::OnOverwriteYesButtonClicked()
{
    //OverwriteOrSaveGame();

    if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
}

// Return to the pause menu without saving
void UPauseMenuWidget::OnOverwriteNoButtonClicked()
{
    if (OverwriteAYS) OverwriteAYS->SetVisibility(ESlateVisibility::Collapsed);
    if (SaveOptionsGroup) SaveOptionsGroup->SetVisibility(ESlateVisibility::Visible);
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
 
/*
//
TArray<FSaveSlotInfo> UPauseMenuWidget::GetAvailableSaveSlots() const
{
    if (UGameInstance* GameInst = GetGameInstance())
    {
        if (UGameSaveSubsystem* SaveSys = GameInst->GetSubsystem<UGameSaveSubsystem>())
        {
            return SaveSys->GetFixedSaveSlots();
        }
    }
    return TArray<FSaveSlotInfo>();
}

//
bool UPauseMenuWidget::OverwriteOrSaveGame(const FString& TargetSlotName)
{
    if (UGameInstance* GameInst = GetGameInstance())
    {
        if (UGameSaveSubsystem* SaveSys = GameInst->GetSubsystem<UGameSaveSubsystem>())
        {
            // Enforce Interface Segregation: Cast strictly down to the Save Contract executor
            if (ISaveExecutor* SaveEngine = Cast<ISaveExecutor>(SaveSys))
            {
                UE_LOG(LogTemp, Log, TEXT("Pause Menu successfully executing save status directly to disk."));
                return SaveEngine->ExecuteSave(TargetSlotName);
            }
        }
    }
    return false;
}
*/