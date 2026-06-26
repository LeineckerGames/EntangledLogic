#include "MainMenuPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EntangledLogic/UI/Menus/MainMenu/MainMenuWidget.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/SettingsMenuWidget.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	// Init Main Menu and add to viewport
	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			// Add to screen
			MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
			MainMenuWidget->AddToViewport();

			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
			SetInputMode(InputMode);

			// Bind the events to the corresponding functions
			MainMenuWidget->OnSettingsButtonClicked.AddUObject(this, &AMainMenuPlayerController::OpenSettingsMenu);
			MainMenuWidget->OnQuitButtonClicked.AddUObject(this, &AMainMenuPlayerController::QuitToDesktop);
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
			SettingsMenuWidget->OnCloseSettingsButtonClicked.AddUObject(this, &AMainMenuPlayerController::CloseSettingsMenu);
		}
	}
}

void AMainMenuPlayerController::OpenSettingsMenu()
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

void AMainMenuPlayerController::CloseSettingsMenu()
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

void AMainMenuPlayerController::QuitToDesktop()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}