#include "TierOneProgressionFactory.h"
#include "Components/WidgetComponent.h"
#include "EntangledLogic/Objects/Factories/Components/FactoryInputComponent.h"
#include "EntangledLogic/Objects/Qubits/Qubit.h"
#include "EntangledLogic/Interfaces/InputOutputInterface.h"
#include "EntangledLogic/Core/Subsystems/FactorySubsystem.h"
#include "EntangledLogic/Core/Subsystems/QubitDataSubsystem.h"
#include "EntangledLogic/Core/Framework/QubitDataStructs.h"
#include "EntangledLogic/UI/Factory/FactoryProgressionUI.h"

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
	UFactorySubsystem* FactorySubsytem = GetWorld()->GetSubsystem<UFactorySubsystem>();
	UQubitDataSubsystem* QubitSubsystem = GetWorld()->GetSubsystem<UQubitDataSubsystem>();
	if (FactorySubsytem && QubitSubsystem)
	{
		int32 Count = 0;
		for (int i = 0; i < NUM_QUBIT_SLOTS; i++)
		{
			if (Qubits[i] != nullptr)
			{
				int32 NumOfProgressionGoals = FactorySubsytem->PersistantStats.CurrentProgressionGoals.Num();
				for (int j = 0; j < NumOfProgressionGoals; j++)
				{
					bool IsQubitEqual = Qubits[i]->State->StateVector.isApprox(FactorySubsytem->PersistantStats.CurrentProgressionGoals[j].GetRequiredKet(), 0.0001);
					//UE_LOG(LogTemp, Display, TEXT("IsQubitEqual for qubit #%d = %d"), i, IsQubitEqual);
					if (IsQubitEqual)
					{
						//UE_LOG(LogTemp, Display, TEXT("Adding aceceptd states"));
						FactorySubsytem->SetProgressionGoalCount(FactorySubsytem->PersistantStats.CurrentProgressionGoals[j],
							FactorySubsytem->PersistantStats.CurrentProgressionGoals[j].ProgressionGoalCount + 1);
					}
					
				}
			}
			Count++;
		}
		UpdateProgressionUI();
	}
}

void ATierOneProgressionFactory::StartProcessingQubits()
{
	
}

void ATierOneProgressionFactory::UpdateProgressionUI()
{
	if (FactoryWidget) 
	{
		UFactoryProgressionUI* FactoryProgressionUI = Cast<UFactoryProgressionUI>(FactoryWidget);
		if (FactoryProgressionUI)
		{
			FactoryProgressionUI->UpdateProgressionUI();
		}
	}
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