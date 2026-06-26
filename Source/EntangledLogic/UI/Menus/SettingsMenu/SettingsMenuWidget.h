#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCloseSettingsButtonClicked);

class UButton;
class UWidgetSwitcher;

UCLASS()
class ENTANGLEDLOGIC_API USettingsMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    FOnCloseSettingsButtonClicked OnCloseSettingsButtonClicked;

protected:
    virtual void NativeConstruct() override;

    // Tab Navigation references
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* MenuWidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    UButton* GameplayTabButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ControlsTabButton;

    UPROPERTY(meta = (BindWidget))
    UButton* GraphicsTabButton;

    UPROPERTY(meta = (BindWidget))
    UButton* AudioTabButton;

    UPROPERTY(meta = (BindWidget))
    UButton* LanguageTabButton;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseButton;

    // On click references
    UFUNCTION()
    void OnGameplayTabClicked();

    UFUNCTION()
    void OnControlsTabClicked();

    UFUNCTION()
    void OnGraphicsTabClicked();

    UFUNCTION()
    void OnAudioTabClicked();

    UFUNCTION()
    void OnLanguageTabClicked();

    UFUNCTION()
    void OnCloseClicked();
};
