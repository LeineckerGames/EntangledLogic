#include "BlackholeFactory.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "Components/SceneCaptureComponent2D.h"

ABlackholeFactory::ABlackholeFactory()
{
	Qubits.SetNum(NUM_QUBIT_SLOTS);
}

// Need to add the deletion of qubits once they transfer

void ABlackholeFactory::OnFactoryTick()
{
	Super::OnFactoryTick();
	UWorld* world = GetWorld();
	if (world)
	{
		UQubitDataSubsystem* QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();

		if (QubitSubsystem) {
			for (int i = 0; i < NUM_QUBIT_SLOTS; i++) {
				if (Qubits[i]) {
					QubitSubsystem->DeleteQubit(*Qubits[i]);
					Qubits[i] = nullptr;
				}
			}
		}
	}
}

void ABlackholeFactory::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
		// Update the prev and next factory pointers
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

		// Update the prev and next factory pointers
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

		// Update the prev and next factory pointers
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

// Input Output Interface
void ABlackholeFactory::ConnectAllInputsAndOutputs()
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

void ABlackholeFactory::ConnectAllInputs()
{
	Super::ConnectAllInputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, false);
	}
}

void ABlackholeFactory::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		ConnectOutputComponent(CurrentOutputComponent, false);
	}
}