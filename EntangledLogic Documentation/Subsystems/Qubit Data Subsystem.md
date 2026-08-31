The Qubit Data Subsystem handles all [[Qubit|Qubit]] state actions and modifications, and serves as the main point of contact with [[Quantum++|Q++]].

### Creating Qubits
`SpawnQubit()`should generally be used for creating qubits, as it will also place them in the world. For uses that don't require the actor in-world, consider using a [[Progression Goals Data Structs#Ket Wrapper|Ket Wrapper]] rather than a [[Qubit|Qubit]].

### Maximum Entanglement
This const represents the maximum size of an entangled group that an operation is allowed to produce. If an operation would produce a larger group, it is cancelled. This is necessary because the time of quantum operations scales exponentially(!) with entangled group size. Currently, the maximum entanglement is set to 3 qubits.

> [!caution] To-Do
> Currently, qubits will simply pass through a gate attempting an operation which would exceed maximum entanglement with no effect. However, ideally, the qubits would become stuck and the gate would show an error, to make this behavior clear to the player.

### Deleting Qubits
Deleting qubits can be finnicky at times due to the limitations on what Unreal's garbage collection does or does not handle automatically.

> [!bug]+ Troubleshooting
> If you're getting crashes you suspect are caused by qubit deletion or memory leaks, check for any pointers or references within AQubit or its members which are not declared as UPROPERTYs, as dangling pointers Unreal doesn't know to clean up can prevent garbage collection. For pointers which can't be UPROPERTYs, such as structs, make sure the pointers are handled, either manually, or automatically with shared pointers or similar. Finally, make sure any external pointers referencing the qubit are handled, such as those in buildings or wires.

### Bloch Vector
`GetBlochVector()`finds the unit Bloch vector for a given qubit, for the purposes of [[Qubit Rendering|rendering]]. Currently, if the Qubit is entangled, it defaults to straight down (the zero state).

>[!help]+ To-Do
>The Bloch sphere model is only originally designed to represent the state of one unentangled qubit, but it may be possible to extend it to multiple qubits with some effort. The game Qubit Factory uses blinking arrows to denote a superposition between two states in 2D, and this could generalize to 3D by drawing two radii. See [[Qubit Rendering#Entangled Qubits]] for more details.
