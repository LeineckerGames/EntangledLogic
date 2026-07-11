#include "TierOneProgressionFactory.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"

ATierOneProgressionFactory::ATierOneProgressionFactory()
{
	Qubits.SetNum(NUM_QUBIT_SLOTS);
}

void ATierOneProgressionFactory::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		// Get the current actor pointers
		AActor* InputSlot0Actor = InputComponents[0]->InputSlot;
		AActor* InputSlot1Actor = InputComponents[1]->InputSlot;
		AActor* InputSlot2Actor = InputComponents[2]->InputSlot;
		AActor* InputSlot3Actor = InputComponents[3]->InputSlot;


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

		if (InputSlot2Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot2Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

		if (InputSlot3Actor)
		{
			IInputOutputInterface* InputSlotActorInputOutputInterface = Cast<IInputOutputInterface>(InputSlot3Actor);
			if (InputSlotActorInputOutputInterface)
			{
				UE_LOG(LogTemp, Display, TEXT("Connect All Outputs Running in destroyed"));
				// Updates the previous factory to connect to the current
				InputSlotActorInputOutputInterface->ConnectAllOutputs();
			}
		}

	}
	Super::EndPlay(EndPlayReason);
}

void ATierOneProgressionFactory::OnFactoryTick()
{
	Super::OnFactoryTick();
	if (IsQubitProcessed)
	{
		bool IsSuccess = OutputQubits();
		if (IsSuccess)
		{
			UE_LOG(LogTemp, Display, TEXT("Outputting Qubits, IsQubitProcessed = false"))
				IsQubitProcessed = false;
		}
	}
}

void ATierOneProgressionFactory::StartProcessingQubits()
{

}

// Input Output Interface
void ATierOneProgressionFactory::ConnectAllInputsAndOutputs()
{
	Super::ConnectAllInputsAndOutputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, true);
	}

	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		ConnectOutputComponent(CurrentOutputComponent, true);
	}
}

void ATierOneProgressionFactory::ConnectAllInputs()
{
	Super::ConnectAllInputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, false);
	}
}

void ATierOneProgressionFactory::ConnectAllOutputs()
{
	// Factory Has No Outputs
}