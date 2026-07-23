#include "MainMenuHUD.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EntangledLogic/UI/Menus/MainMenu/MainMenuWidget.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/SettingsMenuWidget.h"

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

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
			MainMenuWidget->OnSettingsButtonClicked.AddUObject(this, &AMainMenuHUD::OpenSettingsMenu);
			MainMenuWidget->OnQuitButtonClicked.AddUObject(this, &AMainMenuHUD::QuitToDesktop);
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
			SettingsMenuWidget->OnCloseSettingsButtonClicked.AddUObject(this, &AMainMenuHUD::CloseSettingsMenu);
		}
	}
}

// Open the Settings Menu and hide the Main Menu
void AMainMenuHUD::OpenSettingsMenu()
{
	if (!SettingsMenuWidget || !MainMenuWidget) return;

	// Toggle visibility of the widgets
	MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	SettingsMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

// Close the Settings Menu and return to the Main Menu
void AMainMenuHUD::CloseSettingsMenu()
{
	if (!SettingsMenuWidget || !MainMenuWidget) return;

	// Toggle visibility of the widgets
	SettingsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
}

// Quit the game and return to the desktop
void AMainMenuHUD::QuitToDesktop()
{
	APlayerController* PC = GetOwningPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, false);
}

// Function to control the escape button logic
void AMainMenuHUD::EscapeButtonToggle()
{
	// If Credits menu is open, close it and return to main menu, else do the same for settings menu
	if (MainMenuWidget->CreditsPanel && MainMenuWidget->CreditsPanel->IsVisible())
	{
		MainMenuWidget->OnCloseCreditsClicked();
	}
	else if (SettingsMenuWidget && SettingsMenuWidget->IsVisible())
	{
		CloseSettingsMenu();
	}
}