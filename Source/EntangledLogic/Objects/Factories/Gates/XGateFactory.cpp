#include "XGateFactory.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "Components/SceneCaptureComponent2D.h"

AXGateFactory::AXGateFactory()
{

}

// Input Output Interface
void AXGateFactory::ConnectAllInputsAndOutputs()
{
	TArray<UStaticMeshComponent*> InputMeshes = InputOutputComponent->GetInputMeshes();
	TArray<UStaticMeshComponent*> OutputMeshes = InputOutputComponent->GetOutputMeshes();

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> InputCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(InputMeshes);
	TArray<FGridCoordinate> OutputCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(OutputMeshes);

	FGridCoordinate InputSlot0Coord = InputCoordinates[0];
	AActor* InputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(InputSlot0Coord.XCoordinate, InputSlot0Coord.YCoordinate);

	FGridCoordinate OutputSlot0Coord = OutputCoordinates[0];
	AActor* OutputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);

	// Set Pointers if actors are found
	if (InputSlot0Actor)
	{
		InputSlot = InputSlot0Actor;
		IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot);
		if (InputSlotActorInputOutputInterface)
		{
			// Updates the previous factory to connect to the current
			InputSlotActorInputOutputInterface->ConnectAllOutputs();
		}
	}

	if (OutputSlot0Actor)
	{
		OutputSlot = OutputSlot0Actor;
		IInputOutputInterface* OutputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(OutputSlot);
		if (OutputSlotActorInputOutputInterface)
		{
			// Updates the next factory to connect to the current
			OutputSlotActorInputOutputInterface->ConnectAllInputs();
		}
	}

}

void AXGateFactory::ConnectAllInputs()
{

}

void AXGateFactory::ConnectAllOutputs()
{

}