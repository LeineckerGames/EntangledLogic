#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnhancedInputLibrary.h"
#include "EntangledLogic/Interfaces/SavableInterface.h"
#include "EntangledLogic/Core/Framework/GridStructs.h"
#include "EntangledLogic/Core/Framework/SaveDataStructs.h"
#include "GridPlacementSubsystem.generated.h"

UENUM()
enum class EPlacementMode : uint8
{
	Disabled,
	Placing,
	Editing,
	Deletion
};

// Delegates
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlacementModeChanged, EPlacementMode);

UCLASS()
class ENTANGLEDLOGIC_API UGridPlacementSubsystem : public UWorldSubsystem, public ISavableInterface
{
	GENERATED_BODY()

protected:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	TSubclassOf<AActor> SelectedFactoryClass;

	AActor* SelectedFactory;

	float GridSize = 100;

	// Change this to store factory pointer or make a seperate Tmap/ arr for saving
	TMap<FGridCoordinate, AActor*> PlacedPositionMap;

	EPlacementMode PlacementMode = EPlacementMode::Disabled;

	void DeleteSelectedFactory();

	void AddGridPlacementIMC();

	FRotator FactoryCreationRotator = FRotator(0.0f, 0.0f, 0.0f);

	AActor* CreateFactoryFromSaveData(FFactorySaveData FactorySaveData);

	void SetSelectedActorInputOutputMeshesVisible(bool isVisible);

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void SetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition, AActor* PlacedFactory);
	void SetPlacedPositionMap(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape, AActor* PlacedFactory);

	AActor* GetPlacedFactoryAtGridPosition(int32 GridXPosition, int32 GridYPosition);

	bool IsGridPositionOccupied(int32 GridXPosition, int32 GridYPosition);

	float GetGridSize() const;

	void SetSelectedFactory(TSubclassOf<AActor> FactoryClass);

	AActor* SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor);
	AActor* SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor, FTransform SpawnTransform);

	void PlaceSelectedActor();

	void PickupFactory(AActor* FactoryToPickup);

	void DeselectSelectedActor();

	EPlacementMode GetPlacementMode() const;
	
	void SetPlacementMode(EPlacementMode PlacementModeToSet);

	FVector GetWorldGridLocation(FVector Location, FVector GridOffset) const;

	FVector GetGridLocation(FVector Location, FVector GridOffset) const;

	// Can get rid of this once done
	TArray<FGridCoordinate> GetGridPositionsFromInputOutputPlanes(TArray<UStaticMeshComponent*> MeshesToConvert);
	FGridCoordinate GetGridPositionsFromInputOutputPlanes(UStaticMeshComponent* MeshesToConvert);

	TArray<FGridCoordinate> GetGridPositionsFromInputComponents(TArray<class UFactoryInputComponent*> ComponentsToConvert);
	FGridCoordinate GetGridPositionsFromInputComponents(UFactoryInputComponent* ComponentsToConvert);

	TArray<FGridCoordinate> GetGridPositionsFromOutputComponents(TArray<class UFactoryOutputComponent*> ComponentsToConvert);
	FGridCoordinate GetGridPositionsFromOutputComponents(UFactoryOutputComponent* ComponentsToConvert);

	FGridCoordinate GridPositionToCoordinates(FVector GridLocation);

	void MoveSelectedFactoryOnGrid(FVector Location);

	void RotateSelectedActor();

	bool CollisionCheck(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape);

	TArray<FGridCoordinate> GridComponentToCoordinates(class UGridPlacementComponent* GridPlacementComponent);

	void SetPlacementModeToDeletion();
	
	void SetPlacementModeToEditing();

	FOnPlacementModeChanged OnPlacementModeChanged;

	void OnLeftClick();

	void UpdateControlUI();

	void SetFactoryCreationRotator(FRotator Rotator);

	virtual void SaveData(class UFactorySaveGame* SaveGame) override;

	virtual void LoadData(UFactorySaveGame* SaveGame) override;
};
