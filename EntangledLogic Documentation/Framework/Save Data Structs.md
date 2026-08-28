The Save Data Structs file stores the struct definition for `FFactorySaveData`.

## Factory Save Data
The `FFactorySaveData` is used to store all the values that are going to be needed when a new actor is created on the loading of a factory.

The Factory Save Data stores:

[[Grid Structs|FGridCoordinate]] `GridLocation`
[TSubclassOf<*AActor*>](https://dev.epicgames.com/documentation/unreal-engine/typed-object-pointer-properties-in-unreal-engine) `FactoryClass`
[FTransform](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Chaos/FCollectionTransformFacade/Transform) `FactoryTransform`
[FRotator](https://dev.epicgames.com/documentation/unreal-engine/API/Runtime/Core/Math/FRotator_1?application_version=5.5) `FactoryRotation`
