#include "TestingWire.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Objects/WireSegment.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Subsystems/WireSubsystem.h"

ATestingWire::ATestingWire()
{
	// A basic TestingWire doesn't need to tick anymore as segment handles motion
	PrimaryActorTick.bCanEverTick = false;
}

void ATestingWire::BeginPlay()
{
	Super::BeginPlay();
}

void ATestingWire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the wire has been removed from the grid
void ATestingWire::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

		// When the wire is removed from the grid, update wire segments
		UWireSubsystem* WireSubsystem = GetWorld()->GetSubsystem<UWireSubsystem>();
		WireSubsystem->RemoveWireFromPath(this);

	}
	Super::EndPlay(EndPlayReason);
}

// Input Output Interface

// Called when the wire is placed on the grid
void ATestingWire::ConnectAllInputsAndOutputs()
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
	// When wire is placed on the grid, update to wire segments
	UWireSubsystem* WireSubsystem = GetWorld()->GetSubsystem<UWireSubsystem>();
	WireSubsystem->AddWireToPaths(this);
}

void ATestingWire::TransferQubit(AQubit* QubitToTransfer, int32 QubitSlotIndex)
{
	Qubits[QubitSlotIndex] = QubitToTransfer;
	UE_LOG(LogTemp, Display, TEXT("Running AddTestingItemToWire"));
	AssignedSegment->AddTestingItemToWire();
}

void ATestingWire::ConnectAllInputs()
{
	Super::ConnectAllInputs();

	for (UFactoryInputComponent* CurrentInputComponent : InputComponents)
	{
		ConnectInputComponent(CurrentInputComponent, false);
	}

}

void ATestingWire::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		ConnectOutputComponent(CurrentOutputComponent, false);
	}
}

ATestingWire* ATestingWire::GetInputWire()
{
	return Cast<ATestingWire>(InputComponents[0]->InputSlot);
}

ATestingWire* ATestingWire::GetOutputWire()
{
	return Cast<ATestingWire>(OutputComponents[0]->OutputSlot);
}