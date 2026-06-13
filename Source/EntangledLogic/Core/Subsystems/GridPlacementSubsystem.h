// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnhancedInputLibrary.h"
#include "GridPlacementSubsystem.generated.h"

USTRUCT()
struct FGridCoordinate
{
	GENERATED_BODY()
	int32 XCoordinate;
	int32 YCoordinate;

	// Needed to use Struct within Key Value of TMap
	bool operator==(const FGridCoordinate& Other) const
	{
		return XCoordinate == Other.XCoordinate && YCoordinate == Other.YCoordinate;
	}
};

// Needed to use Struct within Key Value of TMap
inline uint32 GetTypeHash(const FGridCoordinate& Coordinate)
{
	// HashCombine merges the hashes of your individual variables
	return HashCombine(::GetTypeHash(Coordinate.XCoordinate), ::GetTypeHash(Coordinate.YCoordinate));
}

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
class ENTANGLEDLOGIC_API UGridPlacementSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	TSubclassOf<AActor> SelectedFactoryClass;

	AActor* SelectedFactory;

	float GridSize = 100;

	// Change this to store factory pointer or make a seperate Tmap/ arr for saving
	TMap<FGridCoordinate, bool> PlacedPositionMap;

	EPlacementMode PlacementMode = EPlacementMode::Disabled;

	void DeleteSelectedFactory() const;

	void AddGridPlacementIMC();

	FRotator FactoryCreationRotator = FRotator(0.0f, 0.0f, 0.0f);

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void SetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition, bool isPlacedToSet);
	void SetPlacedPositionMap(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape, bool isPlacedToSet);

	bool GetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition);

	float GetGridSize() const;

	void SetSelectedFactory(TSubclassOf<AActor> FactoryClass);

	AActor* SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor);

	void PlaceSelectedActor();

	void DeselectSelectedActor();

	EPlacementMode GetPlacementMode() const;
	
	void SetPlacementMode(EPlacementMode PlacementModeToSet);

	FVector GetWorldGridLocation(FVector Location, FVector GridOffset) const;

	FVector GetGridLocation(FVector Location, FVector GridOffset) const;

	void MoveSelectedFactoryOnGrid(FVector Location);

	void RotateSelectedActor();

	bool CollisionCheck(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape);

	TArray<FGridCoordinate> GridComponentToCoordinates(class UGridPlacementComponent* GridPlacementComponent);

	void SetPlacementModeToDeletion();
	
	void SetPlacementModeToEditing();

	FOnPlacementModeChanged OnPlacementModeChanged;

	void OnLeftClick();

	void SetFactoryCreationRotator(FRotator Rotator);
};
