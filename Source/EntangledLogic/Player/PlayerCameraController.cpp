
#include "PlayerCameraController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCameraController::APlayerCameraController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	GridPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridPlane"));
	GridPlane->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

}

// Called when the game starts or when spawned
void APlayerCameraController::BeginPlay()
{
	Super::BeginPlay();
	
	TopDownPlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();

	// Creates Dynamic Grid Material instance
	if (GridMaterial)
	{
		GridMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);
		GridPlane->SetMaterial(0, GridMaterialInstance);
	}

	// Bind Grid Controls
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(CancelPlacement, ETriggerEvent::Completed, GridPlacement, &UGridPlacementSubsystem::DeselectSelectedActor);
		EnhancedInputComponent->BindAction(DeletionMode, ETriggerEvent::Completed, GridPlacement, &UGridPlacementSubsystem::SetPlacementModeToDeletion);
		EnhancedInputComponent->BindAction(EditingMode, ETriggerEvent::Completed, GridPlacement, &UGridPlacementSubsystem::SetPlacementModeToEditing);
		EnhancedInputComponent->BindAction(GridLeftClick, ETriggerEvent::Completed, GridPlacement, &UGridPlacementSubsystem::OnLeftClick);
	}

	// Bind Delegates
	GridPlacement->OnPlacementModeChanged.AddUObject(this, &APlayerCameraController::OnPlacementModeChanged);
}

// Called every frame
void APlayerCameraController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If in placing mode move factory to mouse grid location
	if (GridPlacement->GetPlacementMode() == EPlacementMode::Placing)
	{
		GridPlacement->MoveSelectedFactoryOnGrid(GetWorldMousePosition());
	}
}

// Called to bind functionality to input
void APlayerCameraController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Player Controls
		EnhancedInputComponent->BindAction(KeyboardMovement, ETriggerEvent::Triggered, this, &APlayerCameraController::Move);
		EnhancedInputComponent->BindAction(DragMovement, ETriggerEvent::Triggered, this, &APlayerCameraController::DragMove);
		EnhancedInputComponent->BindAction(PlayerLeftClick, ETriggerEvent::Started, this, &APlayerCameraController::OnLeftClick);
		EnhancedInputComponent->BindAction(PlayerLeftClick, ETriggerEvent::Completed, this, &APlayerCameraController::OnLeftClick);
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
	isDragging = false;
}

FVector APlayerCameraController::GetWorldMousePosition()
{
	FHitResult HitResult;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		bool IsHit = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		if (IsHit)
		{
			FVector WorldLocation = HitResult.Location;
			return WorldLocation;
		}
	}
	return FVector(0.0f, 0.0f, 0.0f);
}

// Used For Drag Movement with Mouse
void APlayerCameraController::DragMove(const FInputActionValue& Value)
{
	if (Controller)
	{
		// Check if dragging
		if (isDragging && GridPlacement->GetPlacementMode() == EPlacementMode::Disabled)
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

void APlayerCameraController::OnPlacementModeChanged(EPlacementMode CurrentPlacementMode)
{
	switch (CurrentPlacementMode)
	{
	case EPlacementMode::Disabled:
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), false);
		break;
	case EPlacementMode::Placing:
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), false);
		break;
	case EPlacementMode::Editing:
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), false);
		break;
	case EPlacementMode::Deletion:
		UE_LOG(LogTemp, Display, TEXT("Placement Mode CHanged to DELETe"));
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), true);
		break;
	}
	
}