#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMainMenuSettingsButtonClicked);
DECLARE_MULTICAST_DELEGATE(FOnMainMenuQuitButtonClicked);

class UButton;
class UTextBlock;
class UWidget;
class USavingLoadingSubsystem;

UCLASS()
class ENTANGLEDLOGIC_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // Broadcast event for when the settings button is clicked
    FOnMainMenuSettingsButtonClicked OnSettingsButtonClicked;

    // Broadcast event for when the quit button is clicked
    FOnMainMenuQuitButtonClicked OnQuitButtonClicked;

protected:
    virtual void NativeConstruct() override;

    // Button references
    UPROPERTY(meta = (BindWidget))
    UButton* AnywhereClickButton;

    UPROPERTY(meta = (BindWidget))
    UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    UButton* LoadScreenBackButton;

    UPROPERTY(meta = (BindWidget))
    UButton* LoadSlot1Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LoadSlot1TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* LoadSlot2Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LoadSlot2TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* LoadSlot3Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LoadSlot3TextBlock;

    UPROPERTY(meta = (BindWidget))
    UButton* LoadSlot4Button;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LoadSlot4TextBlock;

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

	// Sub menu references
    UPROPERTY(meta = (BindWidget))
    UWidget* DeleteOptionsGroup;

    UPROPERTY(meta = (BindWidget))
    UWidget* DeleteAYS;

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

    UPROPERTY(meta = (BindWidget))
    UButton* SettingsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CreditsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseCreditsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    // Sub menu references
    UPROPERTY(meta = (BindWidget))
    UWidget* TitleScreenGroup;

    UPROPERTY(meta = (BindWidget))
    UWidget* MainMenuOptionsGroup;

    UPROPERTY(meta = (BindWidget))
    UWidget* CreditsPanel;

    UPROPERTY(meta = (BindWidget))
    UWidget* LoadOptionsGroup;

    // On click references and functions
    UFUNCTION()
    void OnAnywhereClicked();

    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnLoadScreenBackClicked();

    UFUNCTION()
    void OnLoadSlot1ButtonClicked();

    UFUNCTION()
    void OnLoadSlot2ButtonClicked();

    UFUNCTION()
    void OnLoadSlot3ButtonClicked();

    UFUNCTION()
    void OnLoadSlot4ButtonClicked();

    UFUNCTION()
    void RefreshLoadSlotButtons();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnCreditsClicked();

    UFUNCTION()
    void OnCloseCreditsClicked();

    UFUNCTION()
    void OnQuitGameClicked();

    // The literal asset name of the level map you want to load when clicking Start
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
    FName GameplayLevelName = FName("NickDevLevel");

private:
    USavingLoadingSubsystem* GetSaveSystem() const;

	void TriggerLoad(int32 SlotIndex);

    int32 PendingSaveSlot = -1;
};
