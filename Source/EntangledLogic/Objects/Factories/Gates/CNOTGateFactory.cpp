#include "CNOTGateFactory.h"
#include "Components/SplineComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "Components/SceneCaptureComponent2D.h"

ACNOTGateFactory::ACNOTGateFactory()
{
	Qubits.SetNum(NUM_QUBIT_SLOTS);

	// Setup Qubit Animation Splines
	QubitSplines.Add(CreateDefaultSubobject<USplineComponent>(TEXT("QubitSpline0")));
	QubitSplines.Add(CreateDefaultSubobject<USplineComponent>(TEXT("QubitSpline1")));
	QubitSplines[0]->SetupAttachment(FactoryMesh);
	QubitSplines[1]->SetupAttachment(FactoryMesh);
}

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

void ACNOTGateFactory::OnFactoryTick()
{
	Super::OnFactoryTick();
	if (IsQubitProcessed)
	{
		if (OutputQubits())
		{
			IsQubitProcessed = false;
		}
	}
}

void ACNOTGateFactory::OnQubitProcessed()
{
	Super::OnQubitProcessed();
	UWorld* world = GetWorld();
	if (world)
	{
		UQubitDataSubsystem* QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();
		if (QubitSubsystem)
		{
			UE_LOG(LogTemp, Display, TEXT("Applying the CNOT Gate on the qubit"));
			QubitSubsystem->ApplyControlled(*Qubits[0], *Qubits[1], EQuantumGate::X_Gate);
			CurrentSplineMode = QubitSplineMode::EXIT_MODE;
			UpdateQubitDisplay();
		}
	}
}

// Input Output Interface
void ACNOTGateFactory::ConnectAllInputsAndOutputs()
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

void ACNOTGateFactory::ConnectAllInputs()
{
	Super::ConnectAllInputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, false);
	}

}

void ACNOTGateFactory::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		ConnectOutputComponent(CurrentOutputComponent, false);
	}
}