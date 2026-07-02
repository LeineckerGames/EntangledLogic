#include "CNOTGateFactory.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "Components/SceneCaptureComponent2D.h"

void ACNOTGateFactory::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// Get the current actor pointers
		AActor* InputSlot0Actor = InputComponents[0]->InputSlot;
		AActor* InputSlot1Actor = InputComponents[1]->InputSlot;
		AActor* OutputSlot0Actor = OutputComponents[0]->OutputSlot;
		AActor* OutputSlot1Actor = OutputComponents[1]->OutputSlot;


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

		if (InputSlot1Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot1Actor);
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

		if (OutputSlot1Actor)
		{
			IInputOutputInterface* OutputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(OutputSlot1Actor);
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

// Input Output Interface
void ACNOTGateFactory::ConnectAllInputsAndOutputs()
{
	Super::ConnectAllInputsAndOutputs();

	TArray<UStaticMeshComponent*> InputMeshes;
	//TArray<UStaticMeshComponent*> OutputMeshes;

	// Get the meshes from IO components
	for (UFactoryInputComponent* InputComp : InputComponents)
	{
		InputMeshes.Add(InputComp->GetMesh());
	}

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> InputMeshCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(InputMeshes);
	TArray<FGridCoordinate> InputCoordinates = GridPlacement->GetGridPositionsFromInputComponents(InputComponents);
	TArray<FGridCoordinate> OutputCoordinates = GridPlacement->GetGridPositionsFromOutputComponents(OutputComponents);

	FGridCoordinate InputSlot0Coord = InputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("InputSlot0Coord, X: %d Y: %d"), InputSlot0Coord.XCoordinate, InputSlot0Coord.YCoordinate);

	// use the input mesh to find the previous actor
	FGridCoordinate InputMesh0Coord = InputMeshCoordinates[0];
	AActor* InputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(InputMesh0Coord.XCoordinate, InputMesh0Coord.YCoordinate);

	FGridCoordinate InputSlot1Coord = InputCoordinates[1];
	UE_LOG(LogTemp, Display, TEXT("InputSlot1Coord, X: %d Y: %d"), InputSlot1Coord.XCoordinate, InputSlot1Coord.YCoordinate);

	// use the input mesh to find the previous actor
	FGridCoordinate InputMesh1Coord = InputMeshCoordinates[1];
	AActor* InputSlot1Actor = GridPlacement->GetPlacedFactoryAtGridPosition(InputMesh1Coord.XCoordinate, InputMesh1Coord.YCoordinate);

	FGridCoordinate OutputSlot0Coord = OutputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot0Coord, X: %d Y: %d"), OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);
	AActor* OutputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);

	FGridCoordinate OutputSlot1Coord = OutputCoordinates[1];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot1Coord, X: %d Y: %d"), OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);
	AActor* OutputSlot1Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);

	// Connect Output Slot
	if (OutputSlot0Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot0Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[0]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot0Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[0]->OutputSlot = CurrentInputComponent->GetOwner();

					IInputOutputInterface* PrevFactoryIOInterface = Cast<IInputOutputInterface>(CurrentInputComponent->GetOwner());
					if (PrevFactoryIOInterface)
					{
						PrevFactoryIOInterface->ConnectAllInputs();
					}
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		OutputComponents[0]->OutputSlot = nullptr;
	}
	// Connect Output Slot
	if (OutputSlot1Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot1Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[1]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot1Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[1]->OutputSlot = CurrentInputComponent->GetOwner();

					IInputOutputInterface* PrevFactoryIOInterface = Cast<IInputOutputInterface>(CurrentInputComponent->GetOwner());
					if (PrevFactoryIOInterface)
					{
						PrevFactoryIOInterface->ConnectAllInputs();
					}
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		OutputComponents[1]->OutputSlot = nullptr;
	}

	// Connect Input Slot
	if (InputSlot0Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Input Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(InputSlot0Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Input Interface"));
			FVector InputSlotForwardVector = InputComponents[0]->GetForwardVector();
			TArray<UFactoryOutputComponent*> FactoryOutputComponents = IOInterface->GetOutputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryOutputComponent* CurrentOutputComponent : FactoryOutputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking Output component"));
				FGridCoordinate OutputGridCoord = GridPlacement->GetGridPositionsFromOutputComponents(CurrentOutputComponent);
				FVector OutputSlotForwardVector = CurrentOutputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("OutputGridCoord, X: %d Y: %d"), OutputGridCoord.XCoordinate, OutputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("OutputSlotForwardVector, X: %f Y: %f"), OutputSlotForwardVector.X, OutputSlotForwardVector.Y);

				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && OutputGridCoord == InputSlot0Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					InputComponents[0]->InputSlot = CurrentOutputComponent->GetOwner();

					IInputOutputInterface* PrevFactoryIOInterface = Cast<IInputOutputInterface>(CurrentOutputComponent->GetOwner());
					if (PrevFactoryIOInterface)
					{
						PrevFactoryIOInterface->ConnectAllOutputs();
					}
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		InputComponents[0]->InputSlot = nullptr;
	}

	// Connect Input Slot
	if (InputSlot1Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Input Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(InputSlot1Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Input Interface"));
			FVector InputSlotForwardVector = InputComponents[1]->GetForwardVector();
			TArray<UFactoryOutputComponent*> FactoryOutputComponents = IOInterface->GetOutputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryOutputComponent* CurrentOutputComponent : FactoryOutputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking Output component"));
				FGridCoordinate OutputGridCoord = GridPlacement->GetGridPositionsFromOutputComponents(CurrentOutputComponent);
				FVector OutputSlotForwardVector = CurrentOutputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("OutputGridCoord, X: %d Y: %d"), OutputGridCoord.XCoordinate, OutputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("OutputSlotForwardVector, X: %f Y: %f"), OutputSlotForwardVector.X, OutputSlotForwardVector.Y);

				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && OutputGridCoord == InputSlot1Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					InputComponents[1]->InputSlot = CurrentOutputComponent->GetOwner();

					IInputOutputInterface* PrevFactoryIOInterface = Cast<IInputOutputInterface>(CurrentOutputComponent->GetOwner());
					if (PrevFactoryIOInterface)
					{
						PrevFactoryIOInterface->ConnectAllOutputs();
					}
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		InputComponents[1]->InputSlot = nullptr;
	}
}

void ACNOTGateFactory::ConnectAllInputs()
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
	TArray<FGridCoordinate> InputMeshCoordinates = GridPlacement->GetGridPositionsFromInputOutputPlanes(InputMeshes);
	TArray<FGridCoordinate> InputCoordinates = GridPlacement->GetGridPositionsFromInputComponents(InputComponents);

	FGridCoordinate InputSlot0Coord = InputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("InputSlot0Coord, X: %d Y: %d"), InputSlot0Coord.XCoordinate, InputSlot0Coord.YCoordinate);

	// use the input mesh to find the previous actor
	FGridCoordinate InputMesh0Coord = InputMeshCoordinates[0];
	AActor* InputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(InputMesh0Coord.XCoordinate, InputMesh0Coord.YCoordinate);

	FGridCoordinate InputSlot1Coord = InputCoordinates[1];
	UE_LOG(LogTemp, Display, TEXT("InputSlot1Coord, X: %d Y: %d"), InputSlot1Coord.XCoordinate, InputSlot1Coord.YCoordinate);

	// use the input mesh to find the previous actor
	FGridCoordinate InputMesh1Coord = InputMeshCoordinates[1];
	AActor* InputSlot1Actor = GridPlacement->GetPlacedFactoryAtGridPosition(InputMesh1Coord.XCoordinate, InputMesh1Coord.YCoordinate);

	// Connect Input Slot
	if (InputSlot0Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Input Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(InputSlot0Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Input Interface"));
			FVector InputSlotForwardVector = InputComponents[0]->GetForwardVector();
			TArray<UFactoryOutputComponent*> FactoryOutputComponents = IOInterface->GetOutputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryOutputComponent* CurrentOutputComponent : FactoryOutputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking Output component"));
				FGridCoordinate OutputGridCoord = GridPlacement->GetGridPositionsFromOutputComponents(CurrentOutputComponent);
				FVector OutputSlotForwardVector = CurrentOutputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("OutputGridCoord, X: %d Y: %d"), OutputGridCoord.XCoordinate, OutputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("OutputSlotForwardVector, X: %f Y: %f"), OutputSlotForwardVector.X, OutputSlotForwardVector.Y);

				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && OutputGridCoord == InputSlot0Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					InputComponents[0]->InputSlot = CurrentOutputComponent->GetOwner();
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		InputComponents[0]->InputSlot = nullptr;
	}

	// Connect Input Slot
	if (InputSlot1Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Input Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(InputSlot1Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Input Interface"));
			FVector InputSlotForwardVector = InputComponents[1]->GetForwardVector();
			TArray<UFactoryOutputComponent*> FactoryOutputComponents = IOInterface->GetOutputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryOutputComponent* CurrentOutputComponent : FactoryOutputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking Output component"));
				FGridCoordinate OutputGridCoord = GridPlacement->GetGridPositionsFromOutputComponents(CurrentOutputComponent);
				FVector OutputSlotForwardVector = CurrentOutputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("OutputGridCoord, X: %d Y: %d"), OutputGridCoord.XCoordinate, OutputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("OutputSlotForwardVector, X: %f Y: %f"), OutputSlotForwardVector.X, OutputSlotForwardVector.Y);

				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && OutputGridCoord == InputSlot1Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					InputComponents[1]->InputSlot = CurrentOutputComponent->GetOwner();
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		InputComponents[1]->InputSlot = nullptr;
	}
}

void ACNOTGateFactory::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> OutputCoordinates = GridPlacement->GetGridPositionsFromOutputComponents(OutputComponents);

	FGridCoordinate OutputSlot0Coord = OutputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot0Coord, X: %d Y: %d"), OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);
	AActor* OutputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);

	FGridCoordinate OutputSlot1Coord = OutputCoordinates[1];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot0Coord, X: %d Y: %d"), OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);
	AActor* OutputSlot1Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);


	// Connect Output Slot
	if (OutputSlot0Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot0Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[0]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot0Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[0]->OutputSlot = CurrentInputComponent->GetOwner();
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		OutputComponents[0]->OutputSlot = nullptr;
	}

	// Connect Output Slot
	if (OutputSlot1Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Found Valid Actor"));
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot1Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[1]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot1Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[1]->OutputSlot = CurrentInputComponent->GetOwner();
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		OutputComponents[1]->OutputSlot = nullptr;
	}
}