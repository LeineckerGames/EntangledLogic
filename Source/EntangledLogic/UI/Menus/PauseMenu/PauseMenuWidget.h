#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnResumeButtonClicked);
DECLARE_MULTICAST_DELEGATE(FOnPauseMenuSettingsButtonClicked);
DECLARE_MULTICAST_DELEGATE(FOnQuitToMainMenuButtonClicked);

class UButton;
class UWidget;

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
    UButton* OverwriteYesButton;

    UPROPERTY(meta = (BindWidget))
    UButton* OverwriteNoButton;

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
    void OnOverwriteYesButtonClicked();

    UFUNCTION()
    void OnOverwriteNoButtonClicked();

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

    // UFUNCTION(BlueprintCallable, Category = "PauseMenu|SaveSystem")
    // TArray<FSaveSlotInfo> GetAvailableSaveSlots() const;
    
    // UFUNCTION()
    // bool OverwriteOrSaveGame(const FString& TargetSlotName);
};