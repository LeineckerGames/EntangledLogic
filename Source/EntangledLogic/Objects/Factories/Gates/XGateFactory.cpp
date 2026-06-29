#include "XGateFactory.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "Components/SceneCaptureComponent2D.h"

AXGateFactory::AXGateFactory()
{

}

void AXGateFactory::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// Get the current actor pointers
		AActor* InputSlot0Actor = InputComponents[0]->InputSlot;
		AActor* OutputSlot0Actor = OutputComponents[0]->OutputSlot;


		// Update the prev and next factory pointers
		if (InputSlot0Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot0Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (OutputSlot0Actor)
		{
			IInputOutputInterface* OutputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(OutputSlot0Actor);
			if (OutputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Inputs Running in destroyed"));
				// Updates the next factory to connect to the current
				OutputSlotActorInputOutputInterface->ConnectAllInputs();
			}
		}
	}
	Super::EndPlay(EndPlayReason);
}

void AXGateFactory::Destroyed()
{


	Super::Destroyed();
}

// Input Output Interface
void AXGateFactory::ConnectAllInputsAndOutputs()
{
	Super::ConnectAllInputsAndOutputs();

	TArray<UStaticMeshComponent*> InputMeshes;
	TArray<UStaticMeshComponent*> OutputMeshes;

	// Get the meshes from IO components
	for (UFactoryInputComponent* InputComp : InputComponents)
	{
		InputMeshes.Add(InputComp->GetMesh());
	}

	for (UFactoryOutputComponent* OutputComp : OutputComponents)
	{
		OutputMeshes.Add(OutputComp->GetMesh());
	}

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> InputCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(InputMeshes);
	TArray<FGridCoordinate> OutputCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(OutputMeshes);

	FGridCoordinate InputSlot0Coord = InputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("InputSlot0Coord, X: %d Y: %d"), InputSlot0Coord.XCoordinate, InputSlot0Coord.YCoordinate);
	AActor* InputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(InputSlot0Coord.XCoordinate, InputSlot0Coord.YCoordinate);

	FGridCoordinate OutputSlot0Coord = OutputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot0Coord, X: %d Y: %d"), OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);
	AActor* OutputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);

	// Set Pointers if actors are found
	if (InputSlot0Actor)
	{
		InputComponents[0]->InputSlot = InputSlot0Actor;
		IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputComponents[0]->InputSlot);
		if (InputSlotActorInputOutputInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running"));
			// Updates the previous factory to connect to the current
			InputSlotActorInputOutputInterface->ConnectAllOutputs();
		}
	}
	else
	{
		// Set slot to null if nothing found
		UE_LOG(LogTemp, Display, TEXT("Setting Input Slot to null"));
		InputComponents[0]->InputSlot = nullptr;
	}

	if (OutputSlot0Actor)
	{
		OutputComponents[0]->OutputSlot = OutputSlot0Actor;
		IInputOutputInterface* OutputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(OutputComponents[0]->OutputSlot);
		if (OutputSlotActorInputOutputInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Connect All Inputs Running"));
			// Updates the next factory to connect to the current
			OutputSlotActorInputOutputInterface->ConnectAllInputs();
		}
	}
	else
	{
		// Set slot to null if nothing found
		UE_LOG(LogTemp, Display, TEXT("Setting Output Slot to null"));
		OutputComponents[0]->OutputSlot = nullptr;
	}

}

void AXGateFactory::ConnectAllInputs()
{
	Super::ConnectAllInputs();

	TArray<UStaticMeshComponent*> InputMeshes;

	// Get the meshes from IO components
	for (UFactoryInputComponent* InputComp : InputComponents)
	{
		InputMeshes.Add(InputComp->GetMesh());
	}

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> InputCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(InputMeshes);

	FGridCoordinate InputSlot0Coord = InputCoordinates[0];
	AActor* InputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(InputSlot0Coord.XCoordinate, InputSlot0Coord.YCoordinate);

	// Set Pointers if actors are found
	if (InputSlot0Actor)
	{
		InputComponents[0]->InputSlot = InputSlot0Actor;
	}
	else
	{
		// Set slot to null if nothing found
		UE_LOG(LogTemp, Display, TEXT("Setting Input Slot to null"));
		InputComponents[0]->InputSlot = nullptr;
	}
}

void AXGateFactory::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	TArray<UStaticMeshComponent*> OutputMeshes;

	for (UFactoryOutputComponent* OutputComp : OutputComponents)
	{
		OutputMeshes.Add(OutputComp->GetMesh());
	}

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> OutputCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(OutputMeshes);

	FGridCoordinate OutputSlot0Coord = OutputCoordinates[0];
	AActor* OutputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);

	if (OutputSlot0Actor)
	{
		OutputComponents[0]->OutputSlot = OutputSlot0Actor;
	}
	else
	{
		// Set slot to null if nothing found
		UE_LOG(LogTemp, Display, TEXT("Setting Output Slot to null"));
		OutputComponents[0]->OutputSlot = nullptr;
	}
}