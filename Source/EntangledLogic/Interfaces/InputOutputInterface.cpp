#include "InputOutputInterface.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"

void IInputOutputInterface::ConnectInputComponent(UFactoryInputComponent* InputComponentToConnect, bool ConnectPreviousFactory)
{
	// Convert interface to UObject to get world
	UObject* CurrentUObject = Cast<UObject>(this);
	UWorld* World = CurrentUObject->GetWorld();

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = World->GetSubsystem<UGridPlacementSubsystem>();
	FGridCoordinate InputMeshCoord = GridPlacement->GetGridPositionsFromInputOutputPlanes(InputComponentToConnect->GetMesh());
	FGridCoordinate InputCoord = GridPlacement->GetGridPositionsFromInputComponents(InputComponentToConnect);

	AActor* InputSlotActor = GridPlacement->GetPlacedFactoryAtGridPosition(InputMeshCoord.XCoordinate, InputMeshCoord.YCoordinate);

	// Connect Input Slot
	if (InputSlotActor)
	{
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(InputSlotActor);
		if (IOInterface)
		{
			//UE_LOG(LogTemp, Display, TEXT("Found Input Interface"));
			FVector InputSlotForwardVector = InputComponentToConnect->GetForwardVector();
			TArray<UFactoryOutputComponent*> FactoryOutputComponents = IOInterface->GetOutputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryOutputComponent* CurrentOutputComponent : FactoryOutputComponents)
			{
				//UE_LOG(LogTemp, Display, TEXT("Checking Output component"));
				FGridCoordinate OutputGridCoord = GridPlacement->GetGridPositionsFromOutputComponents(CurrentOutputComponent);
				FVector OutputSlotForwardVector = CurrentOutputComponent->GetForwardVector();
				//UE_LOG(LogTemp, Display, TEXT("OutputGridCoord, X: %d Y: %d"), OutputGridCoord.XCoordinate, OutputGridCoord.YCoordinate);
				//UE_LOG(LogTemp, Display, TEXT("OutputSlotForwardVector, X: %f Y: %f"), OutputSlotForwardVector.X, OutputSlotForwardVector.Y);

				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && OutputGridCoord == InputCoord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					InputComponentToConnect->InputSlot = CurrentOutputComponent->GetOwner();
					if (ConnectPreviousFactory)
					{
						IInputOutputInterface* PrevFactoryIOInterface = Cast<IInputOutputInterface>(CurrentOutputComponent->GetOwner());
						if (PrevFactoryIOInterface)
						{
							PrevFactoryIOInterface->ConnectAllOutputs();
						}
					}
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		InputComponentToConnect->InputSlot = nullptr;
	}

}

void IInputOutputInterface::ConnectOutputComponent(UFactoryOutputComponent* OutputComponentToConnect, bool ConnectPreviousFactory)
{
	// Convert interface to UObject to get world
	UObject* CurrentUObject = Cast<UObject>(this);
	UWorld* World = CurrentUObject->GetWorld();

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = World->GetSubsystem<UGridPlacementSubsystem>();
	FGridCoordinate OutputCoord = GridPlacement->GetGridPositionsFromOutputComponents(OutputComponentToConnect);

	AActor* OutputSlotActor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputCoord.XCoordinate, OutputCoord.YCoordinate);

	// Connect Output Slot
	if (OutputSlotActor)
	{
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlotActor);
		if (IOInterface)
		{
			FVector OutputSlotForwardVector = OutputComponentToConnect->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();

				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputCoord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponentToConnect->OutputSlot = CurrentInputComponent->GetOwner();

					if (ConnectPreviousFactory)
					{
						IInputOutputInterface* PrevFactoryIOInterface = Cast<IInputOutputInterface>(CurrentInputComponent->GetOwner());
						if (PrevFactoryIOInterface)
						{
							PrevFactoryIOInterface->ConnectAllInputs();
						}
					}
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		OutputComponentToConnect->OutputSlot = nullptr;
	}

}


// Temp to fix compiler errors
bool IInputOutputInterface::IsQubitSlotEmpty(int SlotIndex)
{
	// Default implementation
	return false;
}

void IInputOutputInterface::TransferQubit(AQubit* Qubit, int SlotIndex)
{
	// Default implementation
}