#include "PlayerHUD.h"
#include "FactorySelectionWidget.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/UI/Menus/PauseMenu/PauseMenuWidget.h"
#include "EntangledLogic/UI/Menus/SettingsMenu/SettingsMenuWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControlsUI.h"
#include "GoalTracker.h"

APlayerHUD::APlayerHUD()
{

}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* World = GetWorld())
	{
		UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
		GridPlacement->OnPlacementModeChanged.AddUObject(this, &APlayerHUD::UpdatePlayerControlsUI);
		World->OnWorldBeginPlay.AddUObject(this, &APlayerHUD::UpdatePlayerControlsUI);
	}
	UE_LOG(LogTemp, Display, TEXT("hji"));
	

	// If Widget Class is set in editor Create it and add to screen
	if (FactorySelectionWidgetClass)
	{
		FactorySelectionWidget = CreateWidget<UFactorySelectionWidget>(GetWorld(), FactorySelectionWidgetClass);
		FactorySelectionWidget->AddToViewport();
	}

	// If Widget Class is set in editor Create it and add to screen
	if (PlayerControlsUIClass)
	{
		PlayerControlsUIWidget = CreateWidget<UPlayerControlsUI>(GetWorld(), PlayerControlsUIClass);
		PlayerControlsUIWidget->AddToViewport();
		//PlayerControlsUIWidget->UpdatePlayerControlsUI();
	}

	// If Widget Class is set in editor Create it and add to screen
	if (GoalTrackerClass)
	{
		GoalTrackerWidget = CreateWidget<UGoalTracker>(GetWorld(), GoalTrackerClass);
		GoalTrackerWidget->AddToViewport();
	}

	// Init Pause and add to viewport
	if (PauseMenuWidgetClass)
	{
		PauseMenuWidget = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
		if (PauseMenuWidget != nullptr) {
			PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			PauseMenuWidget->AddToViewport();

			// Bind the events to the corresponding functions
			PauseMenuWidget->OnResumeButtonClicked.AddUObject(this, &APlayerHUD::ClosePauseMenu);
			PauseMenuWidget->OnSettingsButtonClicked.AddUObject(this, &APlayerHUD::OpenSettingsMenu);
			PauseMenuWidget->OnQuitToMainMenuButtonClicked.AddUObject(this, &APlayerHUD::QuitToMainMenu);

		}
	}

	// Init Settings and add to viewport
	if (SettingsMenuWidgetClass)
	{
		SettingsMenuWidget = CreateWidget<USettingsMenuWidget>(GetWorld(), SettingsMenuWidgetClass);
		if (SettingsMenuWidget)
		{
			SettingsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			SettingsMenuWidget->AddToViewport();

			// Bind the event to the corresponding function
			SettingsMenuWidget->OnCloseSettingsButtonClicked.AddUObject(this, &APlayerHUD::CloseSettingsMenu);
		}
	}

}

void APlayerHUD::UpdatePlayerControlsUI()
{
	if (PlayerControlsUIWidget)
	{
		PlayerControlsUIWidget->UpdatePlayerControlsUI();
	}
}

void APlayerHUD::UpdatePlayerControlsUI(EPlacementMode PlacementMode)
{
	if (PlayerControlsUIWidget)
	{
		PlayerControlsUIWidget->UpdatePlayerControlsUI();
	}
}

void APlayerHUD::RepopulateFactorySelectionWidget()
{
	if (FactorySelectionWidget)
	{
		FactorySelectionWidget->PopulateInventory();
	}
}

// Toggle the pause and settings menu visibility
void APlayerHUD::TogglePauseMenu()
{
	// If Settings menu is open, close it and return to pause menu
	if (SettingsMenuWidget && SettingsMenuWidget->IsVisible())
	{
		CloseSettingsMenu();
		return;
	}

	// If Settings menu is not open, toggle pause menu visibility
	if (PauseMenuWidget && PauseMenuWidget->IsVisible())
	{
		ClosePauseMenu();
	}
	else
	{
		OpenPauseMenu();
	}
}

// Open the pause menu
void APlayerHUD::OpenPauseMenu()
{
	if (!PauseMenuWidget) return;

	// Toggle visibility
	if (PauseMenuWidget) PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);

	// Pause the game and lock the input to the UI
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->SetPause(true);
		//FInputModeGameAndUI InputMode;
		//InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
		//PC->SetInputMode(InputMode);
	}
}

// Close the pause menu
void APlayerHUD::ClosePauseMenu()
{
	// Toggle visibility
	if (PauseMenuWidget) PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (SettingsMenuWidget) SettingsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	// Unpause the game and unlock the input to the UI
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->SetPause(false);
		//PC->SetInputMode(FInputModeGameOnly());
	}
}

// Open the settings menu
void APlayerHUD::OpenSettingsMenu()
{
	if (!SettingsMenuWidget) return;

	// Toggle visibility
	if (SettingsMenuWidget) SettingsMenuWidget->SetVisibility(ESlateVisibility::Visible);

	// Shift UI focus to the settings menu
	APlayerController* PC = GetOwningPlayerController();
	//if (PC)
	//{
	//	FInputModeGameAndUI InputMode;
	//	InputMode.SetWidgetToFocus(SettingsMenuWidget->TakeWidget());
	//	PC->SetInputMode(InputMode);
	//}
}

// Close the settings menu
void APlayerHUD::CloseSettingsMenu()
{
	if (SettingsMenuWidget) SettingsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (PauseMenuWidget) PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);

	// Shift UI focus to the pause menu
	//APlayerController* PC = GetOwningPlayerController();
	//if (PC)
	//{
	//	FInputModeGameAndUI InputMode;
	//	InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
	//	PC->SetInputMode(InputMode);
	//}
}

// Quit to main menu
void APlayerHUD::QuitToMainMenu()
{
	if (PauseMenuWidget) PauseMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (SettingsMenuWidget) SettingsMenuWidget->SetVisibility(ESlateVisibility::Collapsed);

	// Unpause the game and switch to UI Only
	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		PC->SetPause(false);
		//FInputModeUIOnly InputMode;
		//PC->SetInputMode(InputMode);
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("L_MainMenu"));
}