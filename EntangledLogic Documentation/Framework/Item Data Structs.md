The Item Data Structs is a header only file that describes the information that is used within [Unreal's Data Table](https://dev.epicgames.com/documentation/unreal-engine/data-driven-gameplay-elements-in-unreal-enginea). The Item Data Structs store information for each factory and is used to [[Grid Placement Subsystem|create factories]] and  populate the [[Factory Selection Widget]].

## Item Data
***
The `FItemData` Struct contain two other structs of `FItemTextData` and `FItemAssetData` used for storing information about a factory.
### Item Asset Data
***
`FItemAssetData` stores all the info about the factory itself
#### Icon
Used to store a Texture 2D of a picture of a factory. This is used to within the [[Factory Selection Widget]] to create the button for a factory. 
`UTexture2D* Icon;`

#### Actor Component
The Actor Component stores a [reference to any class](https://dev.epicgames.com/documentation/unreal-engine/typed-object-pointer-properties-in-unreal-engine) of type [AActor](https://dev.epicgames.com/documentation/unreal-engine/actors-in-unreal-engine). This is used to within the [[Grid Placement Subsystem]] to spawn a new factory in the game world.
`TSubclassOf<AActor> ActorComponent;`

>[!note] What Actor to Select
>`TSubclassOf<AActor>` allows you to select any [AActor](https://dev.epicgames.com/documentation/unreal-engine/actors-in-unreal-engine) within the editor. When selecting a factory in the editor make sure to select the Blueprint version of the actor not the C++ version. If you select the C++ actor this will spawn in the factory without any the assets that were added to the blueprint version.

#### Unlock Requirement
The Unlock Requirement stores an [[Unlockables Enum|Unlockable]] which is used as a prerequisite for being able to use the factory.
`EUnlockables UnlockRequirement;`

#### Gate
The Gate stores [[Quantum Gates Enum|Quantum Gate]] type that is used with [[Simple Gate Factory|Simple Gate Factories]] to describe what gate it is.
`EQuantumGate Gate;`

### Item Text Data
The `FItemTextData` stores the any information that will be displayed in any UI's.
***
#### Name
Stores the name of the gate to be shown.
`FText Name;`

#### Description
Stores the description of the factory and what it does.
`FText Description;`

## Factory Data Table
***
Within the editor you will find the **DT_FactoryData** data table where all the information for each factory can be modified. 

You can find **DT_FactoryData** located in the content browser under:
`/Content/Framework/DataTables`

To add a new factory to the data table select the Add button at the top of the menu. Then fill in all the information needed for the factory. The ordering of the rows can be rearranged by dragging the left side of the row. This will change the ordering of how factories appear in the [[Factory Selection Widget]].

>[!note]- Accessing Row Data on Actor
>Within Unreal you can store references to a data table row within the editor allowing for easy access of any row information. This is done within the [[Factory Base]] class to allow each factory to have access to the name and description information to be displayed on the [[Factory UI#Factory Info UI|Factory Info UI]]


**Current Factory Data Table**
![[Example Factory Data table.png|700]]