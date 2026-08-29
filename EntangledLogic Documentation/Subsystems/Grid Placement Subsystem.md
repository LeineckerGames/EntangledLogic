The Grid Placement Subsystem handles the collision and placement of actors on the grid. This is used along side the [[Grid Placement Component]] attached to actors that can be placed on the grid. Using the Grid Placement Subsystem ensures that factories are placed in the correct position locked on the grid no mater the shape of the factory. The subsystem is also used commonly within the [[Player Camera Controller]] to handle the pickup, deletion, and movement of selected actors.

***
The Grid Placement Subsystem has the Enum `EPlacementMode` that has the modes
- **Disabled**
- **Placing**
- **Editing**
- **Deletion**
The Enum is used to store the current placement mode within the variable `PlacementMode`.

***
The Grid Placement Subsystem also has the Delegate `FOnPlacementModeChanged`. This is used to broadcast to other functions when the placement mode is updated.

## Grid Positioning
***
All Grid Positioning is being handled through [Unreal's TMap](https://dev.epicgames.com/documentation/unreal-engine/map-containers-in-unreal-engine) which uses a key-value pair to store locations of actors.

```c++
TMap<FGridCoordinate, AActor*> PlacedPositionMap;
```

The `PlacedPositionMap` stores the [[Grid Structs|FGridCoordinate]] as the key of the map and it stores a reference to an `AActor` that is used to lookup the current actor in the position.

To update the `PlacedPositionMap` you can use the function `SetPlacedPositionMap()`

```c++
void SetPlacedPositionMap(int32 GridXPosition, int32 GridYPosition, AActor* PlacedFactory);

// Override for factories with larger shape
void SetPlacedPositionMap(TArray<FGridCoordinate> GridLocations, TArray<bool> FactoryShape, AActor* PlacedFactory);
```

The `PlacedPositionMap` is also used along side the [[Saving and Loading Subsystem]] to create a [[Save Data Structs|Factory Save Data]] that is used to save factory information.

## Factory Placement
***
The Grid Placement Component handles the movement and the placement of the factory using the `SelectedFactory` which stores an `AActor*`. 

Various functions are used to move or update the `SelectedFactory` these functions include:
```c++
void MoveSelectedFactoryOnGrid(FVector Location);
void RotateSelectedActor();
void PickupFactory(AActor* FactoryToPickup);
void DeselectSelectedActor();
void PlaceSelectedActor();
void SetSelectedFactory(TSubclassOf<AActor> FactoryClass);
AActor* SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor);
AActor* SpawnActorToPlaceFromClass(TSubclassOf<AActor> SelectedActor, FTransform SpawnTransform);
```