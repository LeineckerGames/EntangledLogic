// Fill out your copyright notice in the Description page of Project Settings.

#include "SettingsMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void USettingsMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind button/sliders/selection references to blueprint button/sliders/selection and assign on click events
    if (GameplayTabButton) GameplayTabButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::OnGameplayTabClicked);
    if (ControlsTabButton) ControlsTabButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::OnControlsTabClicked);
    if (GraphicsTabButton) GraphicsTabButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::OnGraphicsTabClicked);
    if (AudioTabButton) AudioTabButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::OnAudioTabClicked);
    if (LanguageTabButton) LanguageTabButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::OnLanguageTabClicked);
    if (CloseButton) CloseButton->OnClicked.AddDynamic(this, &USettingsMenuWidget::OnCloseClicked);
}

// Index 0 matches the Gameplay Canvas/Vertical box sub-panel layout child
void USettingsMenuWidget::OnGameplayTabClicked()
{
    if (MenuWidgetSwitcher)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(0);
    }
}

// Index 1 matches the Controls Canvas/Vertical box sub-panel layout child
void USettingsMenuWidget::OnControlsTabClicked()
{
    if (MenuWidgetSwitcher)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(1);
    }
}

// Index 2 matches the Graphics Canvas/Vertical box sub-panel layout child
void USettingsMenuWidget::OnGraphicsTabClicked()
{
    if (MenuWidgetSwitcher)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(2);
    }
}

// Index 3 matches the Audio Canvas/Vertical box sub-panel layout child
void USettingsMenuWidget::OnAudioTabClicked()
{
    if (MenuWidgetSwitcher)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(3);
    }
}

// Index 4 matches the Language Canvas/Vertical box sub-panel layout child
void USettingsMenuWidget::OnLanguageTabClicked()
{
    if (MenuWidgetSwitcher)
    {
        MenuWidgetSwitcher->SetActiveWidgetIndex(4);
    }
}

// Close SettingsMenu
void USettingsMenuWidget::OnCloseClicked()
{
    OnCloseSettingsButtonClicked.Broadcast();
}