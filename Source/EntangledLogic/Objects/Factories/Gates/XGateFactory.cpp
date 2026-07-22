#include "XGateFactory.h"
#include "Components/SplineComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QuantumGatesEnum.h"
#include "Components/SceneCaptureComponent2D.h"

AXGateFactory::AXGateFactory()
{
	Qubits.SetNum(NUM_QUBIT_SLOTS);

	// Create Qubit movement spline
	QubitSplines.Add(CreateDefaultSubobject<USplineComponent>(TEXT("QubitSpline0")));
	QubitSplines[0]->SetupAttachment(FactoryMesh);
}

// Need to add the deletion of qubits once they transfer

void AXGateFactory::OnFactoryTick()
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

void AXGateFactory::OnQubitProcessed()
{
	Super::OnQubitProcessed();
	UWorld* world = GetWorld();
	if (world)
	{
		UQubitDataSubsystem* QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();
		if (QubitSubsystem)
		{
			//UE_LOG(LogTemp, Display, TEXT("Applying the X Gate on the qubit"))
			QubitSubsystem->Apply(*Qubits[0], EQuantumGate::X_Gate);
			CurrentSplineMode = QubitSplineMode::EXIT_MODE;
			UpdateQubitDisplay();
		}
	}
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

// Input Output Interface
void AXGateFactory::ConnectAllInputsAndOutputs()
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

void AXGateFactory::ConnectAllInputs()
{
	Super::ConnectAllInputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, false);
	}
}

void AXGateFactory::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		ConnectOutputComponent(CurrentOutputComponent, false);
	}
}