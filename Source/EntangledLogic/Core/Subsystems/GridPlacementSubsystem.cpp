#include "GridPlacementSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "EntangledLogic/Core/Components/GridPlacementComponent.h"
#include "EntangledLogic/Interfaces/FactoryInteractionInterface.h"
#include "EntangledLogic/Core/Subsystems/SavingLoadingSubsystem.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Framework/FactorySaveGame.h"
#include "EntangledLogic/Player/TopDownPlayerController.h"
#include "EntangledLogic/Player/PlayerCameraController.h"
#include "EntangledLogic/UI/PlayerHUD.h"


void UGridPlacementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGridPlacementSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// Register Subsystem for saving and loading
	UWorld* World = GetWorld();
	if (World)
	{
		USavingLoadingSubsystem* SavingLoading = World->GetGameInstance()->GetSubsystem<USavingLoadingSubsystem>();
		if (SavingLoading)
		{
			//UE_LOG(LogTemp, Display, TEXT("Registering GPSS to Saving and Loading"));
			SavingLoading->RegisterUObjectToSavingLoading(this);
		}
	}
}

void UGridPlacementSubsystem::SetSelectedFactory(TSubclassOf<AActor> FactoryClass)
{
	DeleteSelectedFactory();

	SelectedFactoryClass = FactoryClass;
	SelectedFactory = SpawnActorToPlaceFromClass(SelectedFactoryClass);

	SetSelectedActorInputOutputMeshesVisible(true);

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

AActor* UGridPlacementSubsystem::SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor, FTransform SpawnTransform)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SelectedActorToPlace = GetWorld()->SpawnActor<AActor>(SelectedActor->GetDefaultObject()->GetClass(), SpawnTransform, SpawnParams);
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

FGridCoordinate UGridPlacementSubsystem::GridPositionToCoordinates(FVector GridLocation)
{
	int32 RoundedX = FMath::FloorToInt(GridLocation.X - 0.5f);
	int32 RoundedY = FMath::FloorToInt(GridLocation.Y - 0.5f);

	FGridCoordinate GridCoordinate = FGridCoordinate(RoundedX, RoundedY);
	return GridCoordinate;
}

void UGridPlacementSubsystem::MoveSelectedFactoryOnGrid(FVector Location)
{
	if (!IsValid(SelectedFactory))
	{
		return;
	}

	UGridPlacementComponent* GridPlacementComponent = SelectedFactory->GetComponentByClass<UGridPlacementComponent>();
	
	if (!IsValid(GridPlacementComponent))
	{
		return;
	}
	
	// Might want to change this not to run every frame
	bool CollisonPass = CollisionCheck(GridComponentToCoordinates(GridPlacementComponent), GridPlacementComponent->GetFactoryShape());
	GridPlacementComponent->UpdateCollisionMaterialParam(!CollisonPass);

	FVector PlacementOffset = GridPlacementComponent->GetPlacementOffset();
	FVector GridLocation = GetWorldGridLocation(Location, PlacementOffset);

	

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
		//UE_LOG(LogTemp, Display, TEXT("Placing Actor"));
		GridPlacementComponent->RemoveOverlayMaterial();
		SetSelectedActorInputOutputMeshesVisible(false);
		SetPlacedPositionMap(GridLocations, GridPlacementComponent->GetFactoryShape(), SelectedFactory);

		// Update Input Outputs
		IInputOutputInterface* SelectedFactoryInputOutputInterface = Cast<IInputOutputInterface>(SelectedFactory);
		if (SelectedFactoryInputOutputInterface)
		{
			SelectedFactoryInputOutputInterface->ConnectAllInputsAndOutputs();
		}
		SelectedFactory = SpawnActorToPlaceFromClass(SelectedFactoryClass);
	}
}

void UGridPlacementSubsystem::PickupFactory(AActor* FactoryToPickup)
{
	SelectedFactory = FactoryToPickup;
	SetSelectedActorInputOutputMeshesVisible(true);
	SelectedFactoryClass = FactoryToPickup->GetClass();
	FactoryCreationRotator = FactoryToPickup->GetActorRotation();
	SetPlacementMode(EPlacementMode::Placing);

	// Update Input Outputs
	IInputOutputInterface* SelectedFactoryInputOutputInterface = Cast<IInputOutputInterface>(SelectedFactory);
	if (SelectedFactoryInputOutputInterface)
	{
		SelectedFactoryInputOutputInterface->ConnectAllInputsAndOutputs();
	}
}

void UGridPlacementSubsystem::DeselectSelectedActor()
{
	//UE_LOG(LogTemp, Display, TEXT("Deselecting Actor"));
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

void UGridPlacementSubsystem::SetSelectedActorInputOutputMeshesVisible(bool isVisible)
{
	IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(SelectedFactory);
	if (IOInterface)
	{
		IOInterface->SetAllInputOutputsVisibility(isVisible);
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
	FGridCoordinate GridCoordinate = GridPositionToCoordinates(GridLocation);

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
	//UE_LOG(LogTemp, Display, TEXT("Placement Mode set to Deletion"));
	DeleteSelectedFactory();
	SetPlacementMode(EPlacementMode::Deletion);
	AddGridPlacementIMC();
}

void UGridPlacementSubsystem::SetPlacementModeToEditing()
{
	//UE_LOG(LogTemp, Display, TEXT("Placement Mode set to Editing"));
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
	
	if (UWorld* World = GetWorld())
	{
		ATopDownPlayerController* TopDownPlayerController = Cast<ATopDownPlayerController>(World->GetFirstPlayerController());
		if (TopDownPlayerController && TopDownPlayerController->GetPawn())
		{
			APlayerCameraController* PlayerCameraController = Cast<APlayerCameraController>(TopDownPlayerController->GetPawn());
			if (PlayerCameraController)
			{
				IFactoryInteractionInterface* CurrentInteraction = PlayerCameraController->GetIFactoryInteractionFromMouse();
				if (CurrentInteraction)
				{
					CurrentInteraction->Interact(PlacementMode);
				}
			}
		}
	}
}

void UGridPlacementSubsystem::AddGridPlacementIMC()
{
	// Add IMC to Player Controller safely
	if (UWorld* World = GetWorld())
	{
		ATopDownPlayerController* TopDownPlayerController = Cast<ATopDownPlayerController>(World->GetFirstPlayerController());
		if (TopDownPlayerController)
		{
			TopDownPlayerController->AddMappingContext(TopDownPlayerController->GridControls, 1);
			UpdateControlUI();
		}
	}
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

TArray<FGridCoordinate> UGridPlacementSubsystem::GetGridPositionsFromInputOutputPlanes(TArray<UStaticMeshComponent*> MeshesToConvert)
{
	TArray<FGridCoordinate> PlaneLocations;
	for (UStaticMeshComponent* CurrentMesh : MeshesToConvert)
	{
		FTransform MeshTransform = CurrentMesh->GetComponentTransform();
		FVector WorldLocation = MeshTransform.GetLocation();
		//UE_LOG(LogTemp, Display, TEXT("CurrentMesh World Location, X: %f Y: %f Z: %f "), WorldLocation.X, WorldLocation.Y, WorldLocation.Z);
		FVector GridLocation = GetGridLocation(WorldLocation, FVector::ZeroVector);
		//UE_LOG(LogTemp, Display, TEXT("CurrentMesh Grid Location, X: %f Y: %f Z: %f "), GridLocation.X, GridLocation.Y, GridLocation.Z);
		FGridCoordinate GridPos = GridPositionToCoordinates(GridLocation);
		PlaneLocations.Add(GridPos);
	}
	return PlaneLocations;
}

FGridCoordinate UGridPlacementSubsystem::GetGridPositionsFromInputOutputPlanes(UStaticMeshComponent* MeshesToConvert)
{
	FTransform MeshTransform = MeshesToConvert->GetComponentTransform();
	FVector WorldLocation = MeshTransform.GetLocation();
	//UE_LOG(LogTemp, Display, TEXT("CurrentMesh World Location, X: %f Y: %f Z: %f "), WorldLocation.X, WorldLocation.Y, WorldLocation.Z);
	FVector GridLocation = GetGridLocation(WorldLocation, FVector::ZeroVector);
	//UE_LOG(LogTemp, Display, TEXT("CurrentMesh Grid Location, X: %f Y: %f Z: %f "), GridLocation.X, GridLocation.Y, GridLocation.Z);
	FGridCoordinate GridPos = GridPositionToCoordinates(GridLocation);
	return GridPos;
}

// Array of Input Component
TArray<FGridCoordinate> UGridPlacementSubsystem::GetGridPositionsFromInputComponents(TArray<UFactoryInputComponent*> ComponentsToConvert)
{
	TArray<FGridCoordinate> IOLocations;
	for (UFactoryInputComponent* CurrentComponenet : ComponentsToConvert)
	{
		FTransform MeshTransform = CurrentComponenet->GetComponentTransform();
		FVector WorldLocation = MeshTransform.GetLocation();
		FVector GridLocation = GetGridLocation(WorldLocation, FVector::ZeroVector);
		FGridCoordinate GridPos = GridPositionToCoordinates(GridLocation);
		IOLocations.Add(GridPos);
	}
	return IOLocations;
}

// Array of Output Component
TArray<FGridCoordinate> UGridPlacementSubsystem::GetGridPositionsFromOutputComponents(TArray<UFactoryOutputComponent*> ComponentsToConvert)
{
	TArray<FGridCoordinate> IOLocations;
	for (UFactoryOutputComponent* CurrentComponenet : ComponentsToConvert)
	{
		FTransform MeshTransform = CurrentComponenet->GetComponentTransform();
		FVector WorldLocation = MeshTransform.GetLocation();
		FVector GridLocation = GetGridLocation(WorldLocation, FVector::ZeroVector);
		FGridCoordinate GridPos = GridPositionToCoordinates(GridLocation);
		IOLocations.Add(GridPos);
	}
	return IOLocations;
}

// Single Input Component
FGridCoordinate UGridPlacementSubsystem::GetGridPositionsFromInputComponents(UFactoryInputComponent* ComponentsToConvert)
{
	FTransform MeshTransform = ComponentsToConvert->GetComponentTransform();
	FVector WorldLocation = MeshTransform.GetLocation();
	FVector GridLocation = GetGridLocation(WorldLocation, FVector::ZeroVector);
	FGridCoordinate GridPos = GridPositionToCoordinates(GridLocation);
	return GridPos;
}

// Single Output Component
FGridCoordinate UGridPlacementSubsystem::GetGridPositionsFromOutputComponents(UFactoryOutputComponent* ComponentsToConvert)
{
	FTransform MeshTransform = ComponentsToConvert->GetComponentTransform();
	FVector WorldLocation = MeshTransform.GetLocation();
	FVector GridLocation = GetGridLocation(WorldLocation, FVector::ZeroVector);
	FGridCoordinate GridPos = GridPositionToCoordinates(GridLocation);
	return GridPos;
}

// Returns nullptr if no GPC is found (shouldn't happen)
AActor* UGridPlacementSubsystem::CreateFactoryFromSaveData(FFactorySaveData FactorySaveData)
{
	// Might want to add nulls checks for each SaveData
	AActor* NewFactory = SpawnActorToPlaceFromClass(FactorySaveData.FactoryClass, FactorySaveData.FactoryTransform);

	// Remove Collision Overaly on spawn
	UGridPlacementComponent* FactoryGPC = NewFactory->GetComponentByClass<UGridPlacementComponent>();
	FactoryGPC->RemoveOverlayMaterial();
	
	// Disable the Input Output arrows at spawn
	//UFactoryInputOutputComponent* InputOutput = NewFactory->GetComponentByClass<UFactoryInputOutputComponent>();
	//InputOutput->SetMeshesVisible(false);

	if (FactoryGPC)
	{
		TArray<FGridCoordinate> GridPositions = GridComponentToCoordinates(FactoryGPC);
		SetPlacedPositionMap(GridPositions, FactoryGPC->GetFactoryShape(), NewFactory);
		return NewFactory;
	}
	return nullptr;
}

void UGridPlacementSubsystem::SaveData(UFactorySaveGame* SaveGame)
{
	TArray<FFactorySaveData> FactorySaveData;

	// Used to check for duplicates
	TSet<AActor*> ProcessedFactories;

	// Create New SaveData for each placed factory
	for (auto& PlacedActorElement : PlacedPositionMap)
	{
		AActor* CurrentFactory = PlacedActorElement.Value;
		if (CurrentFactory && !ProcessedFactories.Contains(CurrentFactory))
		{
			FFactorySaveData NewFactorySaveData;
			NewFactorySaveData.GridLocation = PlacedActorElement.Key;
			NewFactorySaveData.FactoryTransform = CurrentFactory->GetTransform();
			NewFactorySaveData.FactoryRotation = CurrentFactory->GetActorRotation();
			NewFactorySaveData.FactoryClass = CurrentFactory->GetClass();
			ProcessedFactories.Add(CurrentFactory);
			FactorySaveData.Add(NewFactorySaveData);
		}

	}
	SaveGame->SavedFactories = FactorySaveData;
}

void UGridPlacementSubsystem::LoadData(UFactorySaveGame* SaveGame)
{
	TArray<FFactorySaveData> SavedFactories = SaveGame->SavedFactories;
	for (FFactorySaveData CurrentFactoryData : SavedFactories)
	{
		CreateFactoryFromSaveData(CurrentFactoryData);
	}
}

