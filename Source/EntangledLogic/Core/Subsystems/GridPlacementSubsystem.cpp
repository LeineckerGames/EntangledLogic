#include "GridPlacementSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"
#include "EntangledLogic/Interfaces/FactoryInteractionInterface.h"
#include "EntangledLogic/Player/TopDownPlayerController.h"
#include "EntangledLogic/Player/PlayerCameraController.h"
#include "EntangledLogic/UI/PlayerHUD.h"


void UGridPlacementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGridPlacementSubsystem::SetSelectedFactory(TSubclassOf<AActor> FactoryClass)
{
	DeleteSelectedFactory();

	SelectedFactoryClass = FactoryClass;
	SelectedFactory = SpawnActorToPlaceFromClass(SelectedFactoryClass);
	SetPlacementMode(EPlacementMode::Placing);

	AddGridPlacementIMC();
}

AActor* UGridPlacementSubsystem::SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor)
{
	FRotator SpawnRotation = FactoryCreationRotator;
	//FRotator SpawnRotation = FRotator::ZeroRotator;
	//Maybe change this to be 0,0,0 idk
	FVector SpawnLocation = FVector(0, 0, 0);

	FActorSpawnParameters SpawnParams;

	AActor* SelectedActorToPlace = GetWorld()->SpawnActor<AActor>(SelectedActor->GetDefaultObject()->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
	return SelectedActorToPlace;
}

void UGridPlacementSubsystem::SetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition, AActor* PlacedFactory)
{
	FGridCoordinate GridCoordinate;
	GridCoordinate.XCoordinate = GridXPosition;
	GridCoordinate.YCoordinate = GridYPosition;
	
	PlacedPositionMap.Emplace(GridCoordinate, PlacedFactory);
	
}

// Overload for Grid Arrays
void UGridPlacementSubsystem::SetPlacedPositionMap(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape, AActor* PlacedFactory)
{
	int i = 0;
	for (FGridCoordinate GridCoord : GridLocations)
	{
		// Skips factory sections that are open
		if (FactoryShape[i] == true)
		{
			SetPlacedPositionMap(GridCoord.XCoordinate, GridCoord.YCoordinate, PlacedFactory);
			//UE_LOG(LogTemp, Display, TEXT("Set to %d at coordinates X:%d, Y:%d"), PlacedFactory != nullptr, GridCoord.XCoordinate, GridCoord.YCoordinate);
		}
		i++;
	}
}

bool UGridPlacementSubsystem::IsGridPositionOccupied(int32 GridXPosition, int32 GridYPosition)
{
	return GetPlacedFactoryAtGridPosition(GridXPosition, GridYPosition) != nullptr;
}

AActor* UGridPlacementSubsystem::GetPlacedFactoryAtGridPosition(int32 GridXPosition, int32 GridYPosition)
{
	FGridCoordinate GridCoordinate;
	GridCoordinate.XCoordinate = GridXPosition;
	GridCoordinate.YCoordinate = GridYPosition;
	AActor** PlacedFactoryPointer;
	PlacedFactoryPointer = PlacedPositionMap.Find(GridCoordinate);
	if (PlacedFactoryPointer)
	{
		AActor* PlacedFactory = *PlacedFactoryPointer;
		//UE_LOG(LogTemp, Display, TEXT("%d is found at coordinates X:%d, Y:%d"), isPlaced, GridXPosition, GridYPosition);
		return PlacedFactory;
	}
	return nullptr;
}

EPlacementMode UGridPlacementSubsystem::GetPlacementMode() const
{
	return PlacementMode;
}

float UGridPlacementSubsystem::GetGridSize() const
{
	return GridSize;
}

FVector UGridPlacementSubsystem::GetGridLocation(FVector Location, FVector GridOffset) const
{
	float HalfGridSize = GridSize / 2;
	FVector GridPosition = (Location - FVector(HalfGridSize, HalfGridSize, 0.0f) - GridOffset) / GridSize;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Grid Position X:%f, Y:%f"), GridPosition.X, GridPosition.Y));
	return GridPosition;
}

FVector UGridPlacementSubsystem::GetWorldGridLocation(FVector Location, FVector GridOffset) const
{
	float HalfGridSize = GridSize / 2;
	FVector GridPosition = GetGridLocation(Location, GridOffset);
	FVector WorldGridPosition = FVector(
		(FMath::RoundHalfToEven(GridPosition.X) * GridSize) + HalfGridSize,
		(FMath::RoundHalfToEven(GridPosition.Y) * GridSize) + HalfGridSize,
		GridPosition.Z) + GridOffset;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("World Grid Position X:%f, Y:%f"), WorldGridPosition.X, WorldGridPosition.Y));
	return WorldGridPosition;
}

void UGridPlacementSubsystem::MoveSelectedFactoryOnGrid(FVector Location)
{
	UGridPlacementComponent* GridPlacementComponent = SelectedFactory->GetComponentByClass<UGridPlacementComponent>();
	// Might want to change this not to run every frame
	bool CollisonPass = CollisionCheck(GridComponentToCoordinates(GridPlacementComponent), GridPlacementComponent->GetFactoryShape());
	GridPlacementComponent->UpdateCollisionMaterialParam(!CollisonPass);

	FVector PlacementOffset = GridPlacementComponent->GetPlacementOffset();
	FVector GridLocation = GetWorldGridLocation(Location, PlacementOffset);

	if (!IsValid(SelectedFactory))
	{
		return;
	}

	SelectedFactory->SetActorLocation(GridLocation);
}

void UGridPlacementSubsystem::RotateSelectedActor()
{
	if (SelectedFactory)
	{
		FRotator Rotator = FRotator(0.0f, 90.0f, 0.0f);
		FQuat QuatRotation = FQuat(Rotator);
		SelectedFactory->AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
		FactoryCreationRotator = SelectedFactory->GetActorRotation();
	}
}

void UGridPlacementSubsystem::PlaceSelectedActor()
{
	UGridPlacementComponent* GridPlacementComponent = SelectedFactory->GetComponentByClass<UGridPlacementComponent>();
	TArray<FGridCoordinate> GridLocations = GridComponentToCoordinates(GridPlacementComponent);
	bool DidCollide = CollisionCheck(GridLocations, GridPlacementComponent->GetFactoryShape());
	if (DidCollide == false)
	{
		UE_LOG(LogTemp, Display, TEXT("Placing Actor"));
		GridPlacementComponent->RemoveOverlayMaterial();
		SelectedFactory = SpawnActorToPlaceFromClass(SelectedFactoryClass);
		SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), SelectedFactory);
	}
}

void UGridPlacementSubsystem::PickupFactory(AActor* FactoryToPickup)
{
	SelectedFactory = FactoryToPickup;
	SelectedFactoryClass = FactoryToPickup->GetClass();
	FactoryCreationRotator = FactoryToPickup->GetActorRotation();
	SetPlacementMode(EPlacementMode::Placing);
}

void UGridPlacementSubsystem::DeselectSelectedActor()
{
	UE_LOG(LogTemp, Display, TEXT("Deselecting Actor"));
	DeleteSelectedFactory();
	SelectedFactoryClass = nullptr;
	SetPlacementMode(EPlacementMode::Disabled);

	// Crashed here, this should fix but might want to look into more
	if (GetWorld())
	{
		if (ATopDownPlayerController* TopDownPlayerController = Cast<ATopDownPlayerController>(GetWorld()->GetFirstPlayerController()))
		{
			TopDownPlayerController->RemoveMappingContext(TopDownPlayerController->GridControls);
			UpdateControlUI();
		}
	}
}

// Returns True if it collides with something
bool UGridPlacementSubsystem::CollisionCheck(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape)
{
	int i = 0;
	for (FGridCoordinate GridCoord : GridLocations)
	{
		// Skips factory sections that are open
		if (FactoryShape[i] == true)
		{
			bool IsPlacedPosition = IsGridPositionOccupied(GridCoord.XCoordinate, GridCoord.YCoordinate);
			if (IsPlacedPosition == true)
			{
				return true;
			}
		}
		i++;
	}

	// If no collisions are found return
	return false;
}

TArray<FGridCoordinate> UGridPlacementSubsystem::GridComponentToCoordinates(UGridPlacementComponent* GridPlacementComponent)
{
	TArray<FGridCoordinate> GridLocations;
	// Get Factory Size and its Grid Coordinate
	int32 FactorySize = GridPlacementComponent->GetFactorySize();
	FVector GridPlacementComponentLocation = GridPlacementComponent->GetComponentTransform().GetLocation();

	//UE_LOG(LogTemp, Display, TEXT("GridPlacementComponentLocation: X = %f, Y = %f, Z = %f")
	//	, GridPlacementComponentLocation.X, GridPlacementComponentLocation.Y, GridPlacementComponentLocation.Z);

	FVector GridLocation = GetGridLocation(GridPlacementComponentLocation, FVector(0,0,0));
	int32 RoundedX = FMath::FloorToInt(GridLocation.X - 0.5f);
	int32 RoundedY = FMath::FloorToInt(GridLocation.Y - 0.5f);
	//UE_LOG(LogTemp, Display, TEXT("RoundedGridLocation: X = %d, Y = %d"), RoundedX, RoundedY);
	FGridCoordinate GridCoordinate = FGridCoordinate(RoundedX, RoundedY);

	FTransform ComponentTransform = GridPlacementComponent->GetComponentTransform();

	// Get direction vectors
	FVector ForwardDir = ComponentTransform.GetRotation().GetForwardVector();
	FVector RightDir = ComponentTransform.GetRotation().GetRightVector();

	// Convert vectors to ints for direction
	int32 ForwardStepX = FMath::RoundToInt(ForwardDir.X);
	int32 ForwardStepY = FMath::RoundToInt(ForwardDir.Y);
	int32 RightStepX = FMath::RoundToInt(RightDir.X);
	int32 RightStepY = FMath::RoundToInt(RightDir.Y);
	int count = 0;
	for (int i = 0; i < FactorySize; i++)
	{
		for (int j = 0; j < FactorySize; j++)
		{
			int32 AdjustedX = GridCoordinate.XCoordinate + (i * ForwardStepX) + (j * RightStepX);
			int32 AdjustedY = GridCoordinate.YCoordinate + (i * ForwardStepY) + (j * RightStepY);

			FGridCoordinate AdjustedGridCoordinate = FGridCoordinate(AdjustedX, AdjustedY);
			//UE_LOG(LogTemp, Display, TEXT("[%d] Grid Coordinate: X = %d, Y = %d"), count, AdjustedGridCoordinate.XCoordinate, AdjustedGridCoordinate.YCoordinate);
			GridLocations.Add(AdjustedGridCoordinate);
			count++;
		}
	}
	return GridLocations;
}

void UGridPlacementSubsystem::DeleteSelectedFactory() const
{
	// If a factory is currently selected, delete it before selecting new one
	if (SelectedFactory)
	{
		SelectedFactory->Destroy();
	}
}

void UGridPlacementSubsystem::SetPlacementMode(EPlacementMode PlacementModeToSet)
{
	PlacementMode = PlacementModeToSet;
	OnPlacementModeChanged.Broadcast(PlacementMode);
}

void UGridPlacementSubsystem::SetPlacementModeToDeletion()
{
	UE_LOG(LogTemp, Display, TEXT("Placement Mode set to Deletion"));
	DeleteSelectedFactory();
	SetPlacementMode(EPlacementMode::Deletion);
	AddGridPlacementIMC();
}

void UGridPlacementSubsystem::SetPlacementModeToEditing()
{
	UE_LOG(LogTemp, Display, TEXT("Placement Mode set to Editing"));
	DeleteSelectedFactory();
	SetPlacementMode(EPlacementMode::Editing);
	AddGridPlacementIMC();
}

void UGridPlacementSubsystem::SetFactoryCreationRotator(FRotator Rotator)
{
	FactoryCreationRotator = Rotator;
}

void UGridPlacementSubsystem::OnLeftClick()
{
	if (PlacementMode == EPlacementMode::Placing)
	{
		PlaceSelectedActor();
	}
	ATopDownPlayerController* TopDownPlayerController = Cast<ATopDownPlayerController>(GetWorld()->GetFirstPlayerController());
	APlayerCameraController* PlayerCameraController = Cast<APlayerCameraController>(TopDownPlayerController->GetPawn());
	IFactoryInteractionInterface* CurrentInteraction = PlayerCameraController->GetIFactoryInteractionFromMouse();
	if (CurrentInteraction)
	{
		CurrentInteraction->Interact(PlacementMode);
	}
}

void UGridPlacementSubsystem::AddGridPlacementIMC()
{
	// Add IMC to Player Controller
	ATopDownPlayerController* TopDownPlayerController = Cast<ATopDownPlayerController>(GetWorld()->GetFirstPlayerController());
	TopDownPlayerController->AddMappingContext(TopDownPlayerController->GridControls, 1);
	UpdateControlUI();
	UE_LOG(LogTemp, Display, TEXT("Added Grid Mapping Context"));
}

void UGridPlacementSubsystem::UpdateControlUI()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			if (PlayerHUD)
			{
				PlayerHUD->UpdatePlayerControlsUI(PlacementMode);
			}
		}
	}
}

