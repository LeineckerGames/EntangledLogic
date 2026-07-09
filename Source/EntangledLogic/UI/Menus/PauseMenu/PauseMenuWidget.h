#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnResumeButtonClicked);
DECLARE_MULTICAST_DELEGATE(FOnPauseMenuSettingsButtonClicked);
DECLARE_MULTICAST_DELEGATE(FOnQuitToMainMenuButtonClicked);

class UButton;
class UWidget;
class USavingLoadingSubsystem;
class UTextBlock;

UCLASS()
class ENTANGLEDLOGIC_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Broadcast event for when the resume button is clicked
    FOnResumeButtonClicked OnResumeButtonClicked;

    // Broadcast event for when the settings button is clicked
    FOnPauseMenuSettingsButtonClicked OnSettingsButtonClicked;

    // Broadcast event for when the quit to main menu button is clicked
    FOnQuitToMainMenuButtonClicked OnQuitToMainMenuButtonClicked;

protected:
    virtual void NativeConstruct() override;

    // Button references
    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseSaveMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveSlot1Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SaveSlot1TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveSlot2Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SaveSlot2TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveSlot3Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SaveSlot3TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* SaveSlot4Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SaveSlot4TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* OverwriteYesButton;

    UPROPERTY(meta = (BindWidget))
    UButton* OverwriteNoButton;

    UPROPERTY(meta = (BindWidget))
    UButton* DeleteMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseDeleteMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* DeleteSaveSlot1Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeleteSaveSlot1TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* DeleteSaveSlot2Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeleteSaveSlot2TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* DeleteSaveSlot3Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeleteSaveSlot3TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* DeleteSaveSlot4Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DeleteSaveSlot4TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* DeleteYesButton;

    UPROPERTY(meta = (BindWidget))
    UButton* DeleteNoButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseQuitMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitToMainMenuButton;

    /* Commented out because quitting to desktop is kinda sudden. Left here incase a different team wants it.
    // UPROPERTY(meta = (BindWidget))
    // UButton* QuitToDesktopButton;
    */

    // Sub menu references
    UPROPERTY(meta = (BindWidget))
    UWidget* PauseOptionsGroup;

    UPROPERTY(meta = (BindWidget))
    UWidget* SaveOptionsGroup;

    UPROPERTY(meta = (BindWidget))
    UWidget* QuitOptionsGroup;

    UPROPERTY(meta = (BindWidget))
    UWidget* OverwriteAYS;

    UPROPERTY(meta = (BindWidget))
    UWidget* DeleteOptionsGroup;

    UPROPERTY(meta = (BindWidget))
    UWidget* DeleteAYS;

    // On click references and functions
    UFUNCTION()
    void OnResumeClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnSaveMenuClicked();

    UFUNCTION()
    void OnCloseSaveMenuClicked();

    UFUNCTION()
    void OnSaveSlot1Clicked();

    UFUNCTION()
    void OnSaveSlot2Clicked();

    UFUNCTION()
    void OnSaveSlot3Clicked();

    UFUNCTION()
    void OnSaveSlot4Clicked();

    UFUNCTION()
    void RefreshSaveSlotButtons();

    UFUNCTION()
    void OnOverwriteYesButtonClicked();

    UFUNCTION()
    void OnOverwriteNoButtonClicked();

    UFUNCTION()
    void OnDeleteMenuButtonClicked();

    UFUNCTION()
    void OnCloseDeleteMenuButtonClicked();

    UFUNCTION()
    void OnDeleteSaveSlot1Clicked();

    UFUNCTION()
    void OnDeleteSaveSlot2Clicked();

    UFUNCTION()
    void OnDeleteSaveSlot3Clicked();

    UFUNCTION()
    void OnDeleteSaveSlot4Clicked();

    UFUNCTION()
    void OnDeleteYesButtonClicked();

    UFUNCTION()
    void OnDeleteNoButtonClicked();

    UFUNCTION()
    void OnQuitClicked();

    UFUNCTION()
    void OnCloseQuitClicked();

    UFUNCTION()
    void OnMainMenuClicked();

    /* Commented out because quitting to desktop is kinda sudden. Left here incase a different team wants it.
    // UFUNCTION()
    // void OnDesktopClicked();
    */

private:
	int32 PendingSaveSlot = -1;
};