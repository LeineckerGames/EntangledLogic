
#include "PlayerCameraController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"

// Sets default values
APlayerCameraController::APlayerCameraController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

}

// Called when the game starts or when spawned
void APlayerCameraController::BeginPlay()
{
	Super::BeginPlay();
	
	TopDownPlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void APlayerCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCameraController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(KeyboardMovement, ETriggerEvent::Triggered, this, &APlayerCameraController::Move);
		EnhancedInputComponent->BindAction(DragMovement, ETriggerEvent::Triggered, this, &APlayerCameraController::DragMove);
		EnhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Started, this, &APlayerCameraController::OnLeftClick);
		EnhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Completed, this, &APlayerCameraController::OnLeftClick);
	}

}

// Used For WASD movement
void APlayerCameraController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementInput = Value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementInput.Y);
		AddMovementInput(Right, MovementInput.X);
	}
}

void APlayerCameraController::OnLeftClick(const FInputActionValue& Value)
{
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();

	switch (GridPlacement->GetPlacementMode())
	{
		case EPlacementMode::Disabled:
			isDragging = true;
			break;
		case EPlacementMode::Placing:
			// Grid Placement Manager Placing Stuff
			break;
		case EPlacementMode::Editing:
			// Send factory to pickup to grid placement manager
			break;
		case EPlacementMode::Deletion:
			// Grid Placement Manager Deltion Stuff
			break;
	}
}
void APlayerCameraController::OnLeftClickCompleted(const FInputActionValue& Value)
{
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();

	if (GridPlacement->GetPlacementMode() == EPlacementMode::Disabled)
	{
		isDragging = false;
	}
}

// Used For Drag Movement with Mouse
void APlayerCameraController::DragMove(const FInputActionValue& Value)
{
	if (Controller)
	{
		// Check if dragging
		if (isDragging)
		{
			// Get Delta Vector and Normalize it (to make the movement snappy)
			const FVector2D MouseVector = Value.Get<FVector2D>();
			const FVector2D NormalMouseVector = MouseVector.GetSafeNormal();
			//UE_LOG(LogTemp, Display, TEXT("X: %f, Y: %f"), MouseVector.X, MouseVector.Y);

			// Use the value to move the screen the amount the mouse has moved
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(Forward, NormalMouseVector.Y * DragSensitivity, true);
			AddMovementInput(Right, NormalMouseVector.X * DragSensitivity, true);
		}
	}
}
