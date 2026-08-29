The Persistent Stats Struct stores the struct definition for `FPersistantStats`. 

## Persistent Stats
***
The Persistent Stats stores all data that needs to be used with the [[Saving and Loading Subsystem]].
### Current Progression Goals
An array of [[Progression Goals Data Structs|Progression Goals]] that are currently active for the player to complete
`TArray<FProgressionGoal> CurrentProgressionGoals;` 

### Pinned Goal
Stores the Enum value of a [[Progression Goals Data Structs|Progression Goal]] for the currently pinned goal.
`EProgressionGoals PinnedGoal;`

### Gnomes Murdered
Used for the original testing of progression that was done through gnomes.
`int32 GnomesMurdered;` 

### Speed Of Item On Wire
Was made to change the speed of items on wires as a possible future upgrade but not currently used.
`float SpeedOfItemOnWire;` 