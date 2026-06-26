#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnMainMenuSettingsButtonClicked);
DECLARE_MULTICAST_DELEGATE(FOnMainMenuQuitButtonClicked);

class UButton;
class UTextBlock;
class UWidget;

UCLASS()
class ENTANGLEDLOGIC_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    // Broadcast event for when the settings button is clicked
    FOnMainMenuSettingsButtonClicked OnSettingsButtonClicked;

    // Broadcast event for when the quit button is clicked
    FOnMainMenuQuitButtonClicked OnQuitButtonClicked;

    // Function to process the selected save slot and optional custom name
	// TODO: Implement save slot selection processing
    //UFUNCTION()
    //void ProcessSlotSelection(const FSaveSlotInfo& TargetSlot, const FString& CustomNewName);

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
    void OnSettingsClicked();

    UFUNCTION()
    void OnCreditsClicked();

    UFUNCTION()
    void OnCloseCreditsClicked();

    UFUNCTION()
    void OnQuitGameClicked();

    // TODO: Implement save slot retrieval and loading functionality
    //UFUNCTION()
    //TArray<FSaveSlotInfo> GetAvailableSaveSlots() const;

    //UFUNCTION()
    //void LoadSave();

    // TODO: Update this later to load into the loaded save file's level instead of a hardcoded one
    // The literal asset name of the level map you want to load when clicking Start
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
    FName GameplayLevelName = FName("NewMap");
};
