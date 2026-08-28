The Qubit Data Subsystem handles all [[AQubit|Qubit]] state actions and modifications, and serves as the main point of contact with [[Quantum++|Q++]].

### Creating Qubits
`SpawnQubit()`should generally be used for creating qubits, as it will also place them in the world. For uses that don't require the actor in-world, consider using a [[KetWrapper]] rather than a [[AQubit|Qubit]].

### Maximum Entanglement
This const represents the maximum size of an entangled group that an operation is allowed to produce. If an operation would produce a larger group, it is cancelled. This is necessary because the time of quantum operations scales exponentially(!) with entangled group size. Currently, the maximum entanglement is set to 3 qubits.

> [!caution] To-Do
> Currently, qubits will simply pass through a gate attempting an operation which would exceed maximum entanglement with no effect. However, ideally, the qubits would become stuck and the gate would show an error, to make this behavior clear to the player.

### Deleting Qubits
Deleting qubits can be finnicky at times due to the limitations on what Unreal's garbage collection does or does not handle automatically.

> [!bug] Troubleshooting
> If you're getting crashes you suspect are caused by qubit deletion or memory leaks, check for any pointers or references within AQubit or its members which are not declared as UPROPERTYs, as dangling pointers Unreal doesn't know to clean up can prevent garbage collection. For pointers which can't be UPROPERTYs, such as structs, make sure the pointers are handled, either manually, or automatically with shared pointers or similar. Finally, make sure any external pointers referencing the qubit are handled, such as those in buildings or wires.


		void SetState(AQubit& qubit, ENamedState namedState);
		
		void Apply(AQubit& qubit, EQuantumGate gate);
	
		void ApplyControlled(AQubit& control, AQubit& target, EQuantumGate gate);
	
		void SeperateState(AQubit& qubit, std::vector<qpp::idx> Partition);
	
		bool CombineState(AQubit& qubitA, AQubit& qubitB);
	
		void DeleteQubit(AQubit& qubit);
	
		FVector GetBlochVector(AQubit& qubit);
	
	private:
		qqp::ket GetStateAsVector(ENamedState state);
	
		qpp::cmat GetStateAsMatrix(ENamedState state);
	
		qpp::cmat GetGateMatrix(EQuantumGate gate);
