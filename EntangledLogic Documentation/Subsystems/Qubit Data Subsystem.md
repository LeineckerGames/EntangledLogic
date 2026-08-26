The Qubit Data Subsystem handles all [[Qubit]] state actions and modifications, and serves as the main point of contact with [[Quantum++|Q++]].


### Creating Qubits
`SpawnQubit()` should be used for creating qubits


	AQubit* SpawnQubit(FVector SpawnLocation);
	AQubit* SpawnQubit(FVector SpawnLocation, ENamedState namedState);

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
