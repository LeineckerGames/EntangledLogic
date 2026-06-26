#include "MainMenuHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EntangledLogic/UI/Menus/MainMenu/MainMenuWidget.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/SettingsMenuWidget.h"

// Unsure of who does this action, PlayerController, HUD, GameMode, or the MainMenu itself.

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	/*
	// Init Main Menu and add to viewport
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			// Add to screen
			MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
			MainMenuWidget->AddToViewport();

			// Bind the events to the corresponding functions
			MainMenuWidget->OnSettingsButtonClicked.AddDynamic(this, &AMainMenuHUD::OpenSettingsMenu);
			MainMenuWidget->OnQuitButtonClicked.AddDynamic(this, &AMainMenuHUD::QuitToDesktop);
		}
	}

	// Init Settings and add to viewport
	if (SettingsMenuWidgetClass)
	{
		SettingsMenuWidget = CreateWidget<USettingsMenuWidget>(GetWorld(), SettingsMenuWidgetClass);
		if (SettingsMenuWidget)
		{
			// Add to screen and hide initially, will be shown when needed
			SettingsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			SettingsMenuWidget->AddToViewport();

			// Bind the event to the corresponding function
			SettingsMenuWidget->OnCloseSettingsButtonClicked.AddDynamic(this, &AMainMenuHUD::CloseSettingsMenu);
		}
	}*/
}

/*
// Open the Settings Menu and hide the Main Menu
void AMainMenuHUD::OpenSettingsMenu()
{
	if (!SettingsMenuWidget || !MainMenuWidget) return;

	// Toggle visibility of the widgets
	MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	SettingsMenuWidget->SetVisibility(ESlateVisibility::Visible);

	// Shift UI Focus cleanly to Settings
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(SettingsMenuWidget->GetCachedWidget());
	SetInputMode(InputMode);
}

// Close the Settings Menu and return to the Main Menu
void AMainMenuHUD::CloseSettingsMenu()
{
	if (!SettingsMenuWidget || !MainMenuWidget) return;

	// Toggle visibility of the widgets
	SettingsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);

	// Shift UI Focus cleanly back to Main Menu
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->GetCachedWidget());
	SetInputMode(InputMode);
}

// Quit the game and return to the desktop
void AMainMenuHUD::QuitToDesktop()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}
*/