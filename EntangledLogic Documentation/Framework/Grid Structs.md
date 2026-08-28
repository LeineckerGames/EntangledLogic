The Grid Structs stores the definition for the `FGridCoordinate` which is used within the [[Grid Placement Subsystem]].

# Grid Coordinate

The `FGridCoordinate` stores two values for the X and Y position
```
	int32 XCoordinate;
	int32 YCoordinate;
```

Within the struct there is also two functions that are used.

The `operator==()` is used to override the == operator to allow for comparisons between grid coordinates.

The other function `GetTypeHash()` is used to allow Unreal to use the custom struct within the [TMap](https://dev.epicgames.com/documentation/unreal-engine/map-containers-in-unreal-engine).

