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
			Subsystem->AddMappingContext(InputMappingContext, Priority);
			//UE_LOG(LogTemp, Display, TEXT("Player Controls mapping context added"));
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
			Subsystem->RemoveMappingContext(InputMappingContext);
			//UE_LOG(LogTemp, Display, TEXT("Player Controls mapping context added"));
		}
	}
}