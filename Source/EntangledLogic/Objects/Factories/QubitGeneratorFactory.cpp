#include "QubitGeneratorFactory.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "Components/SceneCaptureComponent2D.h"

void AQubitGeneratorFactory::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// Get the current actor pointers
		AActor* OutputSlot0Actor = OutputComponents[0]->OutputSlot;
		AActor* OutputSlot1Actor = OutputComponents[1]->OutputSlot;
		AActor* OutputSlot2Actor = OutputComponents[2]->OutputSlot;
		AActor* OutputSlot3Actor = OutputComponents[3]->OutputSlot;

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

		if (OutputSlot2Actor)
		{
			IInputOutputInterface* OutputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(OutputSlot2Actor);
			if (OutputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Inputs Running in destroyed"));
				// Updates the next factory to connect to the current
				OutputSlotActorInputOutputInterface->ConnectAllInputs();
			}
		}

		if (OutputSlot3Actor)
		{
			IInputOutputInterface* OutputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(OutputSlot3Actor);
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
void AQubitGeneratorFactory::ConnectAllInputsAndOutputs()
{
	Super::ConnectAllInputsAndOutputs();

	UE_LOG(LogTemp, Display, TEXT("VVVVV Qubit Generator VVVVV"));
	TArray<AActor*> OutputActors;

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> OutputCoordinates = GridPlacement->GetGridPositionsFromOutputComponents(OutputComponents);

	FGridCoordinate OutputSlot0Coord = OutputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot0Coord, X: %d Y: %d"), OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);
	AActor* OutputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);

	FGridCoordinate OutputSlot1Coord = OutputCoordinates[1];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot1Coord, X: %d Y: %d"), OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);
	AActor* OutputSlot1Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);

	FGridCoordinate OutputSlot2Coord = OutputCoordinates[2];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot2Coord, X: %d Y: %d"), OutputSlot2Coord.XCoordinate, OutputSlot2Coord.YCoordinate);
	AActor* OutputSlot2Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot2Coord.XCoordinate, OutputSlot2Coord.YCoordinate);

	FGridCoordinate OutputSlot3Coord = OutputCoordinates[3];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot3Coord, X: %d Y: %d"), OutputSlot3Coord.XCoordinate, OutputSlot3Coord.YCoordinate);
	AActor* OutputSlot3Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot3Coord.XCoordinate, OutputSlot3Coord.YCoordinate);

	// Connect Output Slot
	if (OutputSlot0Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Slot 0 Found Valid Actor: %s"), *OutputSlot0Actor->GetActorNameOrLabel());
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
		UE_LOG(LogTemp, Display, TEXT("Slot 1 Found Valid Actor: %s"), *OutputSlot1Actor->GetActorNameOrLabel());
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

	// Connect Output Slot
	if (OutputSlot2Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Slot 2 Found Valid Actor: %s"), *OutputSlot2Actor->GetActorNameOrLabel());
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot2Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[2]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot2Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[2]->OutputSlot = CurrentInputComponent->GetOwner();

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
		OutputComponents[2]->OutputSlot = nullptr;
	}

	// Connect Output Slot
	if (OutputSlot3Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Slot 3 Found Valid Actor: %s"), *OutputSlot3Actor->GetActorNameOrLabel());
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot3Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[3]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot3Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[3]->OutputSlot = CurrentInputComponent->GetOwner();

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
		OutputComponents[3]->OutputSlot = nullptr;
	}

	UE_LOG(LogTemp, Display, TEXT("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"));
}

void AQubitGeneratorFactory::ConnectAllInputs()
{
	// Factory has no inputs
}

void AQubitGeneratorFactory::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	UE_LOG(LogTemp, Display, TEXT("----- Qubit Generator -----"));
	TArray<AActor*> OutputActors;

	// Convert the meshes to grid positions and check adjacent factories
	UGridPlacementSubsystem* GridPlacement = GetWorld()->GetSubsystem<UGridPlacementSubsystem>();
	TArray<FGridCoordinate> OutputCoordinates = GridPlacement->GetGridPositionsFromOutputComponents(OutputComponents);

	FGridCoordinate OutputSlot0Coord = OutputCoordinates[0];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot0Coord, X: %d Y: %d"), OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);
	AActor* OutputSlot0Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot0Coord.XCoordinate, OutputSlot0Coord.YCoordinate);

	FGridCoordinate OutputSlot1Coord = OutputCoordinates[1];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot1Coord, X: %d Y: %d"), OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);
	AActor* OutputSlot1Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot1Coord.XCoordinate, OutputSlot1Coord.YCoordinate);

	FGridCoordinate OutputSlot2Coord = OutputCoordinates[2];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot2Coord, X: %d Y: %d"), OutputSlot2Coord.XCoordinate, OutputSlot2Coord.YCoordinate);
	AActor* OutputSlot2Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot2Coord.XCoordinate, OutputSlot2Coord.YCoordinate);

	FGridCoordinate OutputSlot3Coord = OutputCoordinates[3];
	UE_LOG(LogTemp, Display, TEXT("OutputSlot3Coord, X: %d Y: %d"), OutputSlot3Coord.XCoordinate, OutputSlot3Coord.YCoordinate);
	AActor* OutputSlot3Actor = GridPlacement->GetPlacedFactoryAtGridPosition(OutputSlot3Coord.XCoordinate, OutputSlot3Coord.YCoordinate);

	// Connect Output Slot
	if (OutputSlot0Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Slot 0 Found Valid Actor: %s"), *OutputSlot0Actor->GetActorNameOrLabel());
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
		UE_LOG(LogTemp, Display, TEXT("Slot 1 Found Valid Actor: %s"), *OutputSlot1Actor->GetActorNameOrLabel());
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

	// Connect Output Slot
	if (OutputSlot2Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Slot 2 Found Valid Actor: %s"), *OutputSlot2Actor->GetActorNameOrLabel());
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot2Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[2]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot2Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[2]->OutputSlot = CurrentInputComponent->GetOwner();
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		OutputComponents[2]->OutputSlot = nullptr;
	}

	// Connect Output Slot
	if (OutputSlot3Actor)
	{
		UE_LOG(LogTemp, Display, TEXT("Slot 3 Found Valid Actor: %s"), *OutputSlot3Actor->GetActorNameOrLabel());
		// check if output grid pos = the actors input 
		IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(OutputSlot3Actor);
		if (IOInterface)
		{
			UE_LOG(LogTemp, Display, TEXT("Found Interface"));
			FVector OutputSlotForwardVector = OutputComponents[3]->GetForwardVector();
			TArray<UFactoryInputComponent*> FactoryInputComponents = IOInterface->GetInputComponents();

			// Checks for the same input / output orentation and location
			for (UFactoryInputComponent* CurrentInputComponent : FactoryInputComponents)
			{
				UE_LOG(LogTemp, Display, TEXT("Checking input component"));
				FGridCoordinate InputGridCoord = GridPlacement->GetGridPositionsFromInputComponents(CurrentInputComponent);
				FVector InputSlotForwardVector = CurrentInputComponent->GetForwardVector();
				UE_LOG(LogTemp, Display, TEXT("InputGridCoord, X: %d Y: %d"), InputGridCoord.XCoordinate, InputGridCoord.YCoordinate);
				UE_LOG(LogTemp, Display, TEXT("InputSlotForwardVector, X: %f Y: %f"), InputSlotForwardVector.X, InputSlotForwardVector.Y);
				if (InputSlotForwardVector.Equals(OutputSlotForwardVector, 0.01f) && InputGridCoord == OutputSlot3Coord)
				{
					// Set the pointers if a valid spot
					UE_LOG(LogTemp, Display, TEXT("Found a valid connection"));
					OutputComponents[3]->OutputSlot = CurrentInputComponent->GetOwner();
				}
			}

		}
	}
	else
	{
		// If no factory is found
		UE_LOG(LogTemp, Display, TEXT("No Actor found, Setting slot to null"));
		OutputComponents[3]->OutputSlot = nullptr;
	}
}