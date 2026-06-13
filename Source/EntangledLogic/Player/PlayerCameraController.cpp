
#include "PlayerCameraController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"
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
		EnhancedInputComponent->BindAction(RotateFactory, ETriggerEvent::Completed, GridPlacement, &UGridPlacementSubsystem::RotateSelectedActor);
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

	if (GridPlacement->GetPlacementMode() == EPlacementMode::Editing || GridPlacement->GetPlacementMode() == EPlacementMode::Deletion)
	{
		OutlineHoveredFactory();
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
		EnhancedInputComponent->BindAction(PlayerLeftClick, ETriggerEvent::Completed, this, &APlayerCameraController::OnLeftClickCompleted);
		EnhancedInputComponent->BindAction(PlayerRightClick, ETriggerEvent::Started, this, &APlayerCameraController::OnRightClick);
		EnhancedInputComponent->BindAction(PlayerRightClick, ETriggerEvent::Completed, this, &APlayerCameraController::OnRightClickCompleted);
		EnhancedInputComponent->BindAction(Zoom, ETriggerEvent::Triggered, this, &APlayerCameraController::ZoomCamera);
		EnhancedInputComponent->BindAction(Rotate, ETriggerEvent::Triggered, this, &APlayerCameraController::RotateCamera);
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

void APlayerCameraController::ZoomCamera(const FInputActionValue& Value)
{
	float ZoomDirection = Value.Get<float>();
	
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength + (ZoomDirection * ZoomSpeed), 300.0f, 5000.0f);
}

void APlayerCameraController::RotateCamera(const FInputActionValue& Value)
{
	if (isRotating)
	{
		// Spring Vertical Rotation
		FVector2D RotateVector = Value.Get<FVector2D>() * RotateSensitivity;
		FRotator CurrentSpringArmRotation = SpringArm->GetTargetRotation();
		FRotator SpringArmRotation = FRotator(FMath::Clamp(CurrentSpringArmRotation.Pitch + RotateVector.Y, -80.0f, -10.0f), CurrentSpringArmRotation.Yaw, 0.0f);
		
		// Actor Horizontal Rotation
		FRotator CurrentActorRotation = GetActorRotation();
		FRotator ActorRotation = FRotator(CurrentActorRotation.Pitch, FMath::Clamp(CurrentActorRotation.Yaw + RotateVector.X, -360.0f, 360.0f), 0.0f);
		
		SpringArm->SetWorldRotation(SpringArmRotation);
		SetActorRotation(ActorRotation);
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

void APlayerCameraController::OnRightClick(const FInputActionValue& Value)
{
	isRotating = true;
}

void APlayerCameraController::OnRightClickCompleted(const FInputActionValue& Value)
{
	isRotating = false;
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

// Returns nullptr if no actor is found
AActor* APlayerCameraController::GetHoveredActorFromMousePosition()
{
	FHitResult HitResult;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		bool IsHit = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);
		if (IsHit)
		{
			return HitResult.GetActor();
		}
	}
	return nullptr;
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

			AddMovementInput(Forward, NormalMouseVector.Y * DragSensitivity, false);
			AddMovementInput(Right, NormalMouseVector.X * DragSensitivity, false);
		}
	}
}

void APlayerCameraController::OnPlacementModeChanged(EPlacementMode CurrentPlacementMode)
{
	switch (CurrentPlacementMode)
	{
	case EPlacementMode::Disabled:
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), 2.0f);
		break;
	case EPlacementMode::Placing:
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), 0.0f);
		break;
	case EPlacementMode::Editing:
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), 0.0f);
		break;
	case EPlacementMode::Deletion:
		UE_LOG(LogTemp, Display, TEXT("Placement Mode CHanged to DELETe"));
		GridMaterialInstance->SetScalarParameterValue(FName("DeletionMode"), 1.0f);
		break;
	}
	
}

void APlayerCameraController::OutlineHoveredFactory()
{
	FHitResult HitResult;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		bool IsHit = PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);
		if (IsHit)
		{
			UGridPlacementComponent* FactoryGridPlacementComponent = HitResult.GetActor()->GetComponentByClass<UGridPlacementComponent>();
			if (FactoryGridPlacementComponent)
			{
				if (RecentlyHoveredFactoryGPC)
				{
					RecentlyHoveredFactoryGPC->DisableOutline();
				}
				if (GridPlacement->GetPlacementMode() == EPlacementMode::Editing)
				{
					FactoryGridPlacementComponent->EnableEditOutline();
				}
				if (GridPlacement->GetPlacementMode() == EPlacementMode::Deletion)
				{
					FactoryGridPlacementComponent->EnableDeleteOutline();
				}
				RecentlyHoveredFactoryGPC = FactoryGridPlacementComponent;
			}
			else
			{
				if (RecentlyHoveredFactoryGPC)
				{
					RecentlyHoveredFactoryGPC->DisableOutline();
					RecentlyHoveredFactoryGPC = nullptr;
				}
			}
		}
	}
}