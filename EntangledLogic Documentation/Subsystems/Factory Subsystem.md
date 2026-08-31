The Factory Subsystem is used to broadcast to all factories when a factory tick happens. The Subsystem is also used to keep track of current unlocks and factory progression. These are stored within the [[Persistent Stats Struct#Persistent Stats|PersistentStats]] variable.

## Factory Tick
***
The primary use of Factory Tick is to transfer qubits between factories at a consistent rate.
This is done to ensure that factories are not checking every frame of the game whether a qubit is able to be transferred to the next factory.

Currently the factory ticks every two seconds and broadcasts using the delegate `FOnFactoryTick`. 

> [!note]- Factory Tick on Wires
> The movement of qubits on wires are done using a spline that works similar to a queue where on each factory tick it pops the qubit at the end of the segment. For more information on how wires work you can look at the [[Wire Subsystem]].

Its also possible to use the variable `isTickPaused` to control if the factory ticks are paused. This does not pause the game it only prevents broadcasts of `FOnFactoryTick`.

## Progression Goals
***
In order to make Progression Goals easier to modify and update we decided to use [Unreal's Data Assets](https://dev.epicgames.com/documentation/unreal-engine/data-assets-in-unreal-engine) to allow the modification of progression goals within the editor.

Within the editor there is the [[Progression Goals Data Asset]] where all the info for progression goals are stored. 

Whenever a progression goal is added it gets add to the `CurrentProgressionGoals` array within the [[Persistent Stats Struct#Current Progression Goals|Persistant Stats]]. This is used to keep track of all the active progression goals and used when checking if a valid qubit has entered the a [[Progression Factories|Progression Factory]].

Once a [[Progression Goals Data Structs|Progression Goal]] has been completed add the next progression goals to `CurrentProgressionGoals` and will unlock any factories specified in the [[Progression Goals Data Structs|Progression Goal]]. This then broadcasts to the [[Factory Selection Widget]] and the [[Goal Tracker Widget]] to update any factories / values.

## Progression Unlocks
***
Unlockable are handles through [Unreal's TMap](https://dev.epicgames.com/documentation/unreal-engine/map-containers-in-unreal-engine) called `UnlockablesMap` which takes an [[Unlockables Enum|Unlockable]] for the key and a bool for the value.

```
TMap<EUnlockables, bool> UnlockablesMap;
```

