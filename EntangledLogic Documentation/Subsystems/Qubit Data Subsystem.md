The Qubit Data Subsystem handles all [[Qubit]] state actions and modifications, and serves as the main point of contact with [[Quantum++|Q++]].


### Creating Qubits
`SpawnQubit()`should generally be used for creating qubits, as it will also place them in the world. For uses that don't require the actor in-world, consider using an [[FKetWrapper]] rather than a AQubit.

### Maximum Entanglement
This const represents the maximum size of an entangled group that an operation is allowed to produce. If an operation would produce a larger group, it cannot proceed. This is necessary because the time of quantum operations scales exponentially(!) with entangled group size. Currently, the maximum entanglement is set to 3.

> [!caution] To do
> Currently, qubits will simply pass through a gate attempting an operation which would exceed maximum entanglement with no effect. However, ideally, the qubits would become stuck and the gate would show an error, to make this behavior clear to the player.


	void SetState(AQubit& qubit, ENamedState namedState);

	void Apply(AQubit& qubit, EQuantumGate gate);

	void ApplyControlled(AQubit& control, AQubit& target, EQuantumGate gate);

	void SeperateState(AQubit& qubit, std::vector<qpp::idx> Partition);

	bool CombineState(AQubit& qubitA, AQubit& qubitB);

	void DeleteQubit(AQubit& qubit);

	FVector GetBlochVector(AQubit& qubit);

private:
	qpp::ket GetStateAsVector(ENamedState state);

	qpp::cmat GetStateAsMatrix(ENamedState state);

	qpp::cmat GetGateMatrix(EQuantumGate gate);
