#include "QubitGeneratorFactory.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryOutputComponent.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/GridPlacementSubsystem.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
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

void AQubitGeneratorFactory::OnFactoryTick()
{
	Super::OnFactoryTick();

	int32 SlotNumber = 0;
	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		if (CurrentOutputComponent->OutputSlot)
		{
			AActor* CurrentActor = CurrentOutputComponent->OutputSlot;
			if (CurrentActor)
			{
				UE_LOG(LogTemp, Display, TEXT("Found Actor to send Qubit: %s"), *CurrentActor->GetActorNameOrLabel());
				IInputOutputInterface* IOInterface = Cast<IInputOutputInterface>(CurrentOutputComponent->OutputSlot);
				if (IOInterface)
				{
					// Need a way to get the slot index from other actor and then use it here
					UFactoryInputComponent* ConnectedInputComponent = IOInterface->GetConnectedInputComponent(CurrentOutputComponent);
					if (ConnectedInputComponent)
					{
						int32 InputSlotIndex = ConnectedInputComponent->SlotIndex;
						UE_LOG(LogTemp, Display, TEXT("The input comp of %s has a slot index of %d"), *ConnectedInputComponent->GetOwner()->GetActorNameOrLabel(), InputSlotIndex);
						if (IOInterface->IsQubitSlotEmpty(InputSlotIndex))
						{
							// Create a new qubit and send it
							UQubitDataSubsystem* QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();
							if (QubitSubsystem)
							{
								UE_LOG(LogTemp, Display, TEXT("Creating and Send a new Qubit"));
								IOInterface->TransferQubit(QubitSubsystem->NewQubit(), InputSlotIndex);
							}
						}
					}
				}
			}
			
		}
		SlotNumber++;
	}
}

// Input Output Interface
void AQubitGeneratorFactory::ConnectAllInputsAndOutputs()
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

void AQubitGeneratorFactory::ConnectAllInputs()
{
	Super::ConnectAllInputs();
	// Factory has no inputs
}

void AQubitGeneratorFactory::ConnectAllOutputs()
{
	Super::ConnectAllOutputs();

	for (UFactoryOutputComponent* CurrentOutputComponent : OutputComponents)
	{
		ConnectOutputComponent(CurrentOutputComponent, false);
	}
}