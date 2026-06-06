
#include "TopDownPlayerController.h"
#include "EnhancedInputSubsystems.h"


ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;

}

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		if (PlayerControls)
		{
			Subsystem->AddMappingContext(PlayerControls, 0);
			UE_LOG(LogTemp, Display, TEXT("Player Controls mapping context added"));
		}
	}
}