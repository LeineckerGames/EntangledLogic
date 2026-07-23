#include "TopDownPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EntangledLogic/UI/PlayerHUD.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"


ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;

}

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	APlayerHUD* PlayerHUD = Cast<APlayerHUD>(GetHUD());
	PlayerHUD->UpdatePlayerControlsUI(GridPlacement->GetPlacementMode());

	// SetupInputComponent runs too early so added here for Settings to remap controls
	if (PlayerControls)
	{
		AddMappingContext(PlayerControls, 0);
	}
	if (GridControls)
	{
		AddMappingContext(GridControls, 1);
	}
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	AddMappingContext(PlayerControls, 0);
	UE_LOG(LogTemp, Display, TEXT("Player Controls mapping context added"));

	const FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void ATopDownPlayerController::AddMappingContext(UInputMappingContext* InputMappingContext, int32 Priority)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		if (InputMappingContext)
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = true;

			Subsystem->AddMappingContext(InputMappingContext, Priority, Options);
			//UE_LOG(LogTemp, Display, TEXT("Player Controls mapping context added"));
			if (UEnhancedInputUserSettings* UserSettings = Subsystem->GetUserSettings())
			{
				UserSettings->RegisterInputMappingContext(InputMappingContext);
			}
		}
	}
}

void ATopDownPlayerController::RemoveMappingContext(UInputMappingContext* InputMappingContext)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		if (InputMappingContext)
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = true;

			Subsystem->RemoveMappingContext(InputMappingContext, Options);
			//UE_LOG(LogTemp, Display, TEXT("Player Controls mapping context added"));
		}
	}
}